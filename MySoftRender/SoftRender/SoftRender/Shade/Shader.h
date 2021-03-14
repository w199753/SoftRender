#pragma once
#include<glm/vector_relational.hpp>
#include<glm/matrix.hpp>
#include"V2f.h"
#include"../Render/Vertex.h"
namespace softRD
{
	class Shader
	{
	public:
		Shader() {}
		~Shader() {}

		virtual V2f VertexShader(const Vertex& vertex) = 0;
		//{
		//	V2f res;
		//
		//	return res;
		//}

		virtual glm::vec4 FragmentShader(const V2f& v2f) = 0;
		//{
		//
		//	
		//}
	private:
		
	};
}