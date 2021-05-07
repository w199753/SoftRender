#pragma once
#include"Shader.h"
#include<glm/matrix.hpp>
#include<iostream>
namespace softRD
{
	class SkyboxShader :public Shader
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

			o.worldPos = vertex.position;
			glm::mat4 view_rotMatrix = glm::mat4(glm::mat3(Global::mainCamera->viewMatrix));
			o.windowPos = Global::mainCamera->projectMatrix * view_rotMatrix * vertex.position;
			o.normal = normalMat * vertex.normal;
			o.texcoord = vertex.texcoord;
			o.windowPos.z = o.windowPos.w;
			return o;
		}

		glm::vec4 FragmentShader(const V2f& v2f)override
		{
			glm::vec3 uv = glm::normalize(glm::vec3(v2f.worldPos.x, -v2f.worldPos.y, v2f.worldPos.z));
			glm::vec4 col = block.skybox->SamplerCube(uv);
			if (col.x == 0 && col.y == 0 && col.z == 0)
				return glm::vec4(1);
			return col;
		}
	};
}
