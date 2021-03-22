#pragma once
#include"Shader.h"
#include<glm/matrix.hpp>
#include"../Common/Global.h"
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
			return block.albedo->Sampler2D(v2f.texcoord);
			return glm::vec4(0.2, 0.5, 0.8, 1);
		}

	private:

	};

}
