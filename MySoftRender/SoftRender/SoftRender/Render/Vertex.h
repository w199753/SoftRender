#pragma once
#include<glm/vec4.hpp>
#include<glm/vec3.hpp>
#include<glm/vec2.hpp>
namespace softRD
{
	class Vertex
	{
	public:
		glm::vec4 position;
		glm::vec4 color;
		glm::vec3 normal;
		glm::vec4 tangent;
		glm::vec3 biTangent;
		glm::vec2 texcoord;

		Vertex() {}
		Vertex(const glm::vec4& pos, const glm::vec4& col, const glm::vec3& norm, const glm::vec2& tex) :
			position(pos), color(col), normal(norm), texcoord(tex) {}
		~Vertex() {

		}

	private:

	};

}