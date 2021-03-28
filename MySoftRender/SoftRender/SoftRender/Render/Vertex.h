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

		Vertex() :position(glm::vec4(0)), color(glm::vec4(0)), normal(glm::vec3(0)), tangent(glm::vec4(0)), biTangent(glm::vec3(0)), texcoord(glm::vec2(0)) {

		}
		Vertex(const glm::vec4& pos, const glm::vec4& col, const glm::vec3& norm, const glm::vec2& tex) :
			position(pos), color(col), normal(norm), tangent(glm::vec4(0)), biTangent(glm::vec3(0)), texcoord(tex) {}
		~Vertex() {

		}

	private:

	};

}