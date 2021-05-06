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
			o.worldPos =  vertex.position;
			o.windowPos = Global::mainCamera->viewMatrix* o.worldPos;
			o.normal = normalMat * vertex.normal;
			o.texcoord = vertex.texcoord;
			o.windowPos.z = o.windowPos.w;
			o.Z = 1;
			return o;
		}

		glm::vec4 FragmentShader(const V2f& v2f)override
		{
			glm::vec3 uv = glm::normalize(glm::vec3(v2f.worldPos.x,-v2f.worldPos.y,v2f.worldPos.z));
			return block.skybox->SamplerCube(uv);
		}
	};
}
