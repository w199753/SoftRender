#pragma once

#include"Vertex.h"
//class Vertex;
namespace softRD
{
	class Triangle
	{
	public:

		Triangle(){}
		Triangle(const Vertex& v1,const Vertex& v2,const Vertex& v3);
		~Triangle() {}

		void AddVertices(const Vertex& v1,const Vertex& v2,const Vertex& v3)
		{
			unsigned int idx = 0;
			vertices[idx++] = v1;
			vertices[idx++] = v2;
			vertices[idx++] = v3;
			//�������������������

			//�������������ε���������

		}

		Vertex v0()const { return vertices[0]; }
		Vertex v1()const { return vertices[1]; }
		Vertex v2()const { return vertices[2]; }
	private:
		Vertex vertices[3];

	};

}
