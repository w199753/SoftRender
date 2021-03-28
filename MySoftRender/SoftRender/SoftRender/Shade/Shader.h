#pragma once
#include<glm/vector_relational.hpp>
#include<iostream>
#include<functional>
#include<glm/matrix.hpp>
#include"V2f.h"
#include"../Render/Vertex.h"
#include"../Render/PropertyBlock.h"

namespace softRD
{
	class Pass
	{
	public:
		Pass() {}
		~Pass() {}
		Pass(const std::function<V2f(const Vertex&)>& _v,const std::function<glm::vec4(const V2f&)>& _f)
		{
			vertexShader = _v;
			fragmentShader = _f;
		}
		std::function<V2f(const Vertex&)> vertexShader;
		std::function<glm::vec4(const V2f&)> fragmentShader;
	};
	class Shader
	{
	public:
		std::function<V2f(const Vertex&)> vertexShader;
		std::function<glm::vec4(const V2f&)> fragmentShader;
		Shader(PropertyBlock& _block):block(_block) {}
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

		inline void AddPass(const std::function<V2f(const Vertex&)>& vertexShader,const std::function<glm::vec4(const V2f&)>& fragmentShader)
		{
			pass.push_back(Pass(vertexShader, fragmentShader));
		}

		//void AddPass(std::function<V2f(Shader&, const Vertex&)> vertexShader, std::function<glm::vec4(Shader&, const V2f&)> fragmentShader)
		//{
		//	pass[0].vertexShader = vertexShader;
		//	pass[0].fragmentShader = fragmentShader;
		//}
		PropertyBlock& block;
		std::vector<Pass> pass;
	private:
		
	};
}