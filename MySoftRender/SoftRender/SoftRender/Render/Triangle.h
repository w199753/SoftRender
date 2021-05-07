#pragma once
#include"Vertex.h"
#include"../Math/fMath.h"
#include <glm/trigonometric.hpp>

//class Vertex;
namespace softRD
{
	class Triangle
	{
	public:

		Triangle() {}
		Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
		~Triangle() {}

		void AddVertices(const Vertex& v1, const Vertex& v2, const Vertex& v3)
		{
			unsigned int idx = 0;
			vertices[idx++] = v1;
			vertices[idx++] = v2;
			vertices[idx++] = v3;
			//三角形三个点添加完了

			//构建其他三角形及其顶点的所需属性
			for (int i = 0; i < 3; i++)
			{
				CalTangent(i, (i + 1) % 3, (i + 2) % 3);
			}

		}

		void CalTangent(int a, int b, int c)
		{
			glm::vec3 n = vertices[a].normal;
			glm::vec3 e1 = vertices[b].position - vertices[a].position;
			glm::vec3 e2 = vertices[c].position - vertices[a].position;

			float delta_u1 = vertices[b].texcoord.x - vertices[a].texcoord.x;
			float delta_u2 = vertices[c].texcoord.x - vertices[a].texcoord.x;
			float delta_v1 = vertices[b].texcoord.y - vertices[a].texcoord.y;
			float delta_v2 = vertices[c].texcoord.y - vertices[a].texcoord.y;


			float r = 1.0f / (delta_v1 * delta_u2 - delta_v2 * delta_u1);
			//glm::vec3 tan1 = (delta_v1*e2 - delta_v2*e1)*r;
			glm::vec3 tan1 = (delta_v2 * e1 - delta_v1 * e2) * r;
			glm::vec3 tan2 = (delta_u1 * e2 - delta_u2 * e1) * r;

			auto t = tan1;
			glm::vec4 tangent = glm::vec4(glm::normalize((t - n * glm::dot(n, t))), 1);
			tangent.w = (glm::dot(glm::cross(n, t), tan2) < 0.0f) ? -1.0f : 1.0f;
			vertices[a].tangent = tangent;
			vertices[a].biTangent = glm::cross(glm::normalize(n), glm::vec3(tangent)) * tangent.w;
		}

		Vertex v0()const { return vertices[0]; }
		Vertex v1()const { return vertices[1]; }
		Vertex v2()const { return vertices[2]; }
	private:
		Vertex vertices[3];

	};

}
