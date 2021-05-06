#pragma once
#include"Shader.h"
#include<glm/matrix.hpp>
#include<iostream>
namespace softRD
{
	class SkyBoxPreCompute :public Shader
	{
	public:
		SkyBoxPreCompute(PropertyBlock& _block) :Shader(_block) {
			AddPass(std::bind(&SkyBoxPreCompute::VertexShader, this, std::placeholders::_1), std::bind(&SkyBoxPreCompute::FragmentShader, this, std::placeholders::_1));
		}
		~SkyBoxPreCompute() {}

		V2f VertexShader(const Vertex& vertex) override
		{
			V2f o;
			auto modelMatrix = block.transform->translateMatrix * block.transform->rotateMatrix * block.transform->scaleMatrix;
			glm::mat3x3 normalMat = glm::mat3x3(modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2],
				modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2],
				modelMatrix[2][0], modelMatrix[2][1], modelMatrix[2][2]);
			o.worldPos = modelMatrix * vertex.position;
			o.windowPos = Global::mainCamera->projectMatrix * Global::mainCamera->viewMatrix * o.worldPos;
			o.normal = normalMat * vertex.normal;
			o.texcoord = vertex.texcoord;
			return o;
		}

		glm::vec4 FragmentShader(const V2f& v2f)override
		{
			glm::vec2 invAtan(0.1591, 0.3183);
			glm::vec3 v = glm::normalize(glm::vec3(v2f.worldPos));
			glm::vec2 uv = glm::vec2(atan2(v.z, v.x), asin(v.y));
			uv *= invAtan;
			uv += 0.5f;
			return block.albedo->Sampler2D(uv);
			return glm::vec4(0.2, 0.5, 0.8, 1);
		}
	};
}