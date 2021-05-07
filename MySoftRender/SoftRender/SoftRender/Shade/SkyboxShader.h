#pragma once
#include"Shader.h"
#include<glm/matrix.hpp>
#include<iostream>
namespace softRD
{
	class SkyboxShader:public Shader
	{
	public:
		SkyboxShader(PropertyBlock& _block) :Shader(_block) {
			AddPass(std::bind(&SkyboxShader::VertexShader, this, std::placeholders::_1), std::bind(&SkyboxShader::FragmentShader, this, std::placeholders::_1));
		}
		~SkyboxShader() {}

		V2f VertexShader(const Vertex& vertex) override
		{
			V2f o;
			auto modelMatrix = block.transform->translateMatrix * block.transform->rotateMatrix * block.transform->scaleMatrix;
			glm::mat3x3 normalMat = glm::mat3x3(modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2],
				modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2],
				modelMatrix[2][0], modelMatrix[2][1], modelMatrix[2][2]);
			
			glm::mat4 hh = glm::mat4(1);
			hh[3][0] = Global::mainCamera->position.x;
			hh[3][1] = Global::mainCamera->position.y;
			hh[3][2] = Global::mainCamera->position.z;
			o.worldPos = vertex.position;
			//std::cout << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << std::endl;
			glm::mat4 vv = glm::mat4(glm::mat3(Global::mainCamera->viewMatrix));
			glm::vec4 pp =  Global::mainCamera->projectMatrix*vv*  vertex.position;
			o.normal = normalMat * vertex.normal;
			o.texcoord = vertex.texcoord;
			o.windowPos = glm::vec4(pp.x, pp.y, max(0.02f,pp.w), max(0.02f,pp.w));
			//o.windowPos = glm::vec4(pp.x, pp.y,  pp.w,  pp.w);
			//if (pp.w<0.0000001)
			//{
			//	std::cout << "1111" << std::endl;
			//}
			o.Z = 1;
			return o;
		}

		glm::vec4 FragmentShader(const V2f& v2f)override
		{
			//return glm::vec4(1);
			glm::vec3 uv = glm::normalize(glm::vec3(v2f.worldPos.x,-v2f.worldPos.y,v2f.worldPos.z));
			glm::vec4 col = block.skybox->SamplerCube(uv);
			if (col.x == 0 && col.y == 0 && col.z == 0)
				return glm::vec4(1);
			return col;
		}
	};
}
