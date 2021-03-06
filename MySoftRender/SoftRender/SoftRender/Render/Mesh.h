#pragma once
#include<vector>
#include<iostream>
#include<glm/vector_relational.hpp>
#include"Vertex.h"
#include"Triangle.h"
namespace softRD
{

	class Mesh
	{
	public:
		Mesh() { triangleStream.reserve(100); }
		~Mesh() {}

		inline void PushVertex(const Vertex& v)
		{
			//std::cout << v.position.x<<" "<<v.position.y<<" "<<v.position.z << std::endl;
			//std::cout << v.normal.x << " " << v.normal.y << " " << v.normal.z << std::endl;
			//std::cout << v.texcoord.x << " " << v.texcoord.y << " "<< std::endl;
			//std::cout << " --------------------------- "  << std::endl;
			Vertices.push_back(v);
		}

		inline void PushIndices(const int idx)
		{
			Indices.push_back(idx);
		}

		void BuildTriangleStream()
		{
			for (int i = 0; i < Indices.size(); i += 3)
			{
				Triangle* t = new Triangle();
				t->AddVertices(Vertices[i], Vertices[i + 1], Vertices[i + 2]);
				triangleStream.emplace_back(t);
			}
		}

		


		inline int getVectorVerticesSize() const { return Vertices.size(); }

		inline void setVerticesCount(int c) { verticesCount = c; }
		inline int getVerticesCount()const { return verticesCount; }

		inline int getTrianglesCount()const { return Indices.size() / 3; }

		inline const std::vector<Triangle*>& getTriangleStream() const { return triangleStream; }
	private:
		int verticesCount = 0;
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;

		std::vector<Triangle*> triangleStream;
	};

}