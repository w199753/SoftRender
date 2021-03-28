#pragma once
#include<glm/vec4.hpp>
#include<glm/vec3.hpp>
#include<glm/vec2.hpp>
namespace softRD
{
	class Vertex
	{
	public:
		glm::vec4 position = glm::vec4(0);
		glm::vec4 color = glm::vec4(0);
		glm::vec3 normal = glm::vec3(0);
		glm::vec4 tangent = glm::vec4(0);
		glm::vec3 biTangent = glm::vec3(0);
		glm::vec2 texcoord = glm::vec2(0);

		Vertex() {

		}
		Vertex(const glm::vec4& pos, const glm::vec4& col, const glm::vec3& norm, const glm::vec2& tex) :
			position(pos), color(col), normal(norm), texcoord(tex) {}
		~Vertex() {

		}

	private:

	};

}