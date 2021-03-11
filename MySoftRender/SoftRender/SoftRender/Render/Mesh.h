#pragma once
#include<vector>
#include"Vertex.h"
#include"Triangle.h"
namespace softRD
{

	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		inline void PushVertex(const Vertex& v)
		{
			Vertices.push_back(v);
		}

		inline void PushIndices(const int idx)
		{
			Indices.push_back(idx);
		}

		void BuildTriangleStream()
		{
			Triangle* t = new Triangle();
			for (size_t i = 0; i < Indices.size(); i += 3)
			{
				t->AddVertices(Vertices[i], Vertices[i + 1], Vertices[i + 2]);
			}
			triangleStream.push_back(t);
		}

		int getVerticesCount()const { return Vertices.size(); }

		int getTrianglesCount()const { return Indices.size() / 3; }

		std::vector<Triangle*> getTriangleStream() const { return triangleStream; }
	private:
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;

		std::vector<Triangle*> triangleStream;
	};

}