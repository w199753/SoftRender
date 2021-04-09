#pragma once
#include"Shader.h"
#include<glm/matrix.hpp>
#include"../Common/Global.h"
#include"../Light/Light.h"
#include<iostream>
using namespace std;
namespace softRD
{

	class PhongShader :public Shader
	{
	public:

		//using Shader::Shader;
		PhongShader(PropertyBlock& _block) :Shader(_block) {
			AddPass(std::bind(&PhongShader::VertexShader, this, std::placeholders::_1), std::bind(&PhongShader::FragmentShader, this, std::placeholders::_1));
		}
		~PhongShader() {}

		V2f VertexShader(const Vertex& vertex) override
		{
			V2f o;

			auto modelMatrix = block.transform->translateMatrix * block.transform->rotateMatrix * block.transform->scaleMatrix;
			glm::mat3x3 normalMat = glm::mat3x3(modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2],
				modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2],
				modelMatrix[2][0], modelMatrix[2][1], modelMatrix[2][2]);
			//cout << v.position << endl;
			//cout << modelMatrix << endl;
			//cout << v.position << endl;
			o.worldPos = modelMatrix * vertex.position;
			//cout << vertex.position.x<<" "<<vertex.position.y<<" "<<vertex.position.z << "        " << o.worldPos.x<<" "<< o.worldPos.y<<" "<< o.worldPos.z << endl;
			//cout << o.worldPos<<">>>"<<v.position << endl;
			o.windowPos = Global::mainCamera->projectMatrix * Global::mainCamera->viewMatrix * o.worldPos;
			//cout << "??????????---------" << o.windowPos.x << " " << o.windowPos.y << " " << o.windowPos.z << " " << o.windowPos.w << endl;
			o.normal = normalMat * vertex.normal;
			o.texcoord = vertex.texcoord;

			return o;
		}

		glm::vec4 FragmentShader(const V2f& v2f)override
		{
			//return glm::vec4(v2f.windowPos.z);
			glm::vec4 res_Col = glm::vec4(0);
			glm::vec4 col = block.albedo->Sampler2D(v2f.texcoord);

			auto N = glm::normalize(v2f.normal);
			for (size_t index = 0; index < Global::pointLightList.size(); index++)
			{
				auto L = glm::normalize(Global::pointLightList[index]->position - glm::vec3(v2f.worldPos));
				auto c = Global::pointLightList[index]->color;
				auto i = Global::pointLightList[index]->intensity;
				auto a = Global::pointLightList[index]->getAttenuation(v2f.worldPos);
				res_Col += col * std::max(0.001f, glm::dot(L,N)) * glm::vec4(c, 1) * i * a;
			}
			for (int index = 0; index < Global::dirLightList.size(); index++)
			{
				auto L = glm::normalize(Global::dirLightList[index]->dirction);
				auto i = Global::dirLightList[index]->intensity;
				auto c = Global::dirLightList[index]->color;
				auto a = Global::dirLightList[index]->getAttenuation(v2f.worldPos);
				res_Col += col * std::max(0.001f, glm::dot(L,N)) * glm::vec4(c, 1) * i * a;
			}

			//std::cout << v2f.worldPos.x<<" "<< v2f.worldPos .y<<" "<< v2f.worldPos .z<<" "<< v2f.worldPos .w<< " " << std::endl;
			return res_Col;
			return glm::vec4(0.2, 0.5, 0.8, 1);
		}

	private:

	};

}
