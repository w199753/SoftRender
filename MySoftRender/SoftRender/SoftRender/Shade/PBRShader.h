#pragma once
#include"Shader.h"
#include<glm/matrix.hpp>
#include"../Common/Global.h"
#include"../Light/Light.h"
#include<iostream>
using namespace std;
namespace softRD
{
	class PBRShader :public Shader
	{
	public:
		const float pi = 3.1415926f;
		const float one_div_pi = 0.31830989f;
		//using Shader::Shader;
		PBRShader(PropertyBlock& _block) :Shader(_block) {
			AddPass(std::bind(&PBRShader::VertexShader, this, std::placeholders::_1), std::bind(&PBRShader::FragmentShader, this, std::placeholders::_1));
		}
		~PBRShader() {}

		V2f VertexShader(const Vertex& vertex) override
		{
			V2f o;
			auto modelMatrix = block.transform->translateMatrix * block.transform->rotateMatrix * block.transform->scaleMatrix;
			glm::mat3x3 normalMat = glm::mat3x3(modelMatrix);
			normalMat = glm::transpose(glm::inverse(normalMat));
			o.worldPos = modelMatrix * vertex.position;
			o.windowPos = Global::mainCamera->projectMatrix * Global::mainCamera->viewMatrix * o.worldPos;

			o.normal = glm::normalize(normalMat * glm::vec3(vertex.normal));
			glm::vec3 tangent = glm::normalize(normalMat * vertex.tangent);
			glm::vec3 bitangent = glm::normalize(glm::cross(o.normal, tangent)* vertex.tangent.w);
			o.tbn = glm::mat3x3(tangent, bitangent, o.normal);
			o.texcoord = vertex.texcoord;

			return o;
		}

		glm::vec4 FragmentShader(const V2f& v2f)override
		{
			//return glm::vec4(1);
			glm::vec4 albedo = block.albedo->Sampler2D(v2f.texcoord);
			float ao = block.ao->Sampler2D(v2f.texcoord).r;
			//return albedo;
			//glm::vec3 N = fMath::UnPackNormal(block.normal->Sampler2D(v2f.texcoord));
			glm::vec3 N = glm::normalize(v2f.normal);
			glm::vec3 L_dir = glm::normalize(Global::dirLightList[0]->dirction);
			glm::vec3 V = glm::normalize(Global::mainCamera->position - glm::vec3(v2f.worldPos));
			glm::vec3 N_half = glm::normalize(N + V);

			float metallic = block.metallic->Sampler2D(v2f.texcoord).r;
			float roughness = block.metallic->Sampler2D(v2f.texcoord).r;

			float NdotH = std::max(0.f,glm::dot(N, N_half));
			float NdotV = std::max(glm::dot(N, V),0.f);
			float HdotV = std::max(glm::dot(N_half, V), 0.f);
			float NdotL = std::max(glm::dot(N, L_dir), 0.f);

			glm::vec4 dirLight = CalDirLight(albedo, V, N, L_dir, roughness, metallic, NdotH, NdotV, HdotV, NdotL);
			glm::vec4 inDirLight = CalInDirLight(albedo,v2f.worldPos,V,N,metallic,roughness,NdotV);
			return (dirLight+inDirLight)*ao;
		}

		float NormalDistributionFunciont_GGX(float roughness,float NdotH)
		{
			float roughnessSqr = roughness * roughness;
			float roughnessFourSqr = roughnessSqr * roughnessSqr;
			float NdotHSqr = NdotH * NdotH;
			return (roughnessFourSqr / ((NdotHSqr * (roughnessFourSqr - 1) + 1) * (NdotHSqr * (roughnessFourSqr - 1) + 1)))* one_div_pi;
		}

		float GeometryFunction_GGX(float NdotV,float roughness)
		{
			float r = (roughness + 1.0f);
			float k = (r * r) / 8.0f;

			float nom = NdotV;
			float denom = NdotV * (1.0f - k) + k;
			return nom / denom;
		}

		float GeometrySmith(float NdotV, float NdotL, float roughness)
		{
			float ggx2 = GeometryFunction_GGX(NdotV, roughness);
			float ggx1 = GeometryFunction_GGX(NdotL, roughness);
			return ggx2 * ggx1;
		}

		glm::vec4 FresnelSchlick_GGX(glm::vec4& F0,float HdotV)
		{
			return F0 + (1.0f - F0) * pow(1.0f - HdotV, 5.0f);
		}

		//计算直接光照部分
		glm::vec4 CalDirLight(glm::vec4 & albedo,glm::vec3& V,glm::vec3& N,glm::vec3& L_dir,const float roughness,const float metallic, float NdotH,float NdotV,float HdotV,float NdotL)
		{
			
			glm::vec4 F0(0.04);
			F0 = fMath::Lerp(F0, albedo, metallic);

			float D = NormalDistributionFunciont_GGX(roughness, NdotH);
			float G = GeometrySmith(NdotV, NdotL,roughness);
			glm::vec4 F = FresnelSchlick_GGX(F0,HdotV);
			glm::vec4 ks = F;
			glm::vec4 kd = glm::vec4(1) - ks;
			kd *= (1.0f - metallic);
			float denom = (4.0f * NdotV * NdotL +0.0001f);
			glm::vec4 specular = (D*G*ks)/denom;

			glm::vec4 diffuse = albedo * kd;
			return specular;
			return (diffuse+specular)*NdotL*Global::dirLightList[0]->intensity*glm::vec4(Global::dirLightList[0]->color,1);
		}

		glm::vec4 fresnelSchlickRoughness(float cosTheta, glm::vec4 F0, float roughness)
		{
			return F0 + (max(glm::vec4(1) * (1 - roughness), F0) - F0) * pow(1.0f - cosTheta, 5.0f);
		}
		glm::vec4 CalInDirLight(glm::vec4 albedo,const glm::vec3& worldPos, const glm::vec3& V,const glm::vec3& N,float metallic,float roughness, float NdotV)
		{
			glm::vec3 normal = glm::normalize(worldPos);
			glm::vec4 diffuse(0);
			glm::vec4 specular(0);

			glm::vec4 F0 = fMath::Lerp(glm::vec4(0.04), albedo, metallic);
			glm::vec4 ks = fresnelSchlickRoughness(NdotV,F0, roughness);
			glm::vec4 kd = glm::vec4(1) - ks;
			kd *= (1.0 - metallic);

			glm::vec3 R = glm::reflect(-V, N);
			glm::vec4 radiance = block.radiance->SamplerCube(glm::vec3(R.x,-R.y,R.z));
			glm::vec4 brdfRes = block.brdf->Sampler2D(glm::vec2(NdotV, roughness));
			glm::vec2 envBrdf(brdfRes.x, brdfRes.y);
			glm::vec4 brdfSpecualr = radiance * (ks * envBrdf.x + envBrdf.y);
			glm::vec4 irradiance = block.irradiance->SamplerCube(glm::vec3(N.x,-N.y,N.z));
			diffuse = (albedo * irradiance * kd + brdfSpecualr);
			//std::cout << normal.x << " " << normal.y << " " << normal.z << " " << std::endl;
			return diffuse;
		}

	private:
	};
}