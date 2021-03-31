#pragma once
#include"Shader.h"
#include<glm/matrix.hpp>
#include"../Common/Global.h"
#include"../Render/Camera.h"
namespace softRD
{
	class UnlitShader:public Shader
	{
	public:

		//using Shader::Shader;
		UnlitShader(PropertyBlock& _block) :Shader(_block) {
			AddPass(std::bind(&UnlitShader::VertexShader, this, std::placeholders::_1), std::bind(&UnlitShader::FragmentShader, this, std::placeholders::_1));
		}
		~UnlitShader() {}

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
			o.windowPos = Global::mainCamera->projectMatrix * Global::mainCamera->viewMatrix * o.worldPos;
			o.color = vertex.color;

			return o;
		}

		glm::vec4 FragmentShader(const V2f& v2f)override
		{
			return v2f.color;
		}

	private:

	};
}