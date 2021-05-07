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

		//using Shader::Shader;
		PBRShader(PropertyBlock& _block) :Shader(_block) {
			AddPass(std::bind(&PBRShader::VertexShader, this, std::placeholders::_1), std::bind(&PBRShader::FragmentShader, this, std::placeholders::_1));
		}
		~PBRShader() {}

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

			return glm::vec4(0.2, 0.5, 0.8, 1);
		}

	private:
	};
}