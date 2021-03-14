#pragma once
#include"Mesh.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<glm/vector_relational.hpp>
#include <string>
#include<vector>
namespace softRD
{
	class ObjModelLoader
	{
	public:
		ObjModelLoader(Mesh& _mesh) :mesh(_mesh)
		{

		}
		~ObjModelLoader()
		{

		}
		Mesh& LoadModelMesh(const std::string& path)
		{
			bool hasNormal = false;
			int verticesCount = 0;
			std::cout << path << std::endl;
			std::ifstream in;
			in.open(path, std::ios::in);
			if (in.fail())
			{
				std::cout << "no file!" << std::endl;
				return mesh;
			}
			std::string line;
			std::vector<glm::vec3> position;
			std::vector<glm::vec3> normal;
			std::vector<glm::vec2> texcoord;

			while (!in.eof())
			{
				std::getline(in, line);
				std::istringstream iss(line);
				char trash;
				if (!line.compare(0, 2, "v "))
				{
					verticesCount++;
					iss >> trash;
					glm::vec3 pos;
					iss >> pos.x;
					iss >> pos.y;
					iss >> pos.z;
					//std::cout << "pos:" << pos.x << " " << pos.y << " " << pos.z << std::endl;
					position.push_back(pos);
				}
				else if (!line.compare(0, 3, "vn "))
				{
					hasNormal = true;
					iss >> trash >> trash;
					glm::vec3 norm;
					iss >> norm.x;
					iss >> norm.y;
					iss >> norm.z;
					normal.push_back(norm);
				}
				else if (!line.compare(0, 3, "vt "))
				{
					iss >> trash >> trash;
					glm::vec2 tex;
					iss >> tex.x;
					iss >> tex.y;
					texcoord.push_back(tex);
				}
				else if (!line.compare(0, 2, "f "))
				{

					iss >> trash;
					float v[4][3];
					//float vextra[3];
					if (hasNormal == false)  //部分搞pbr得模型没有normal，依靠法线贴图
					{
						for (int i = 0; i < 3; i++)
						{
							Vertex vert;
							iss >> v[i][0] >> trash >> v[i][1];
							vert.position = glm::vec4(position[v[i][0]-1], 1);
							vert.texcoord = texcoord[v[i][1]-1];
							mesh.PushVertex(vert);
							mesh.PushIndices(mesh.getVectorVerticesSize());
						}
						if (iss >> v[3][0])
						{
							iss >> trash >> v[3][1];
							Vertex vert1;
							vert1.position = glm::vec4(position[v[0][0]-1], 1);
							vert1.texcoord = texcoord[v[0][1]-1];
							mesh.PushVertex(vert1);
							mesh.PushIndices(mesh.getVectorVerticesSize());
							Vertex vert2;
							vert2.position = glm::vec4(position[v[2][0]-1], 1);
							vert2.texcoord = texcoord[v[2][1]-1];
							mesh.PushVertex(vert2);
							mesh.PushIndices(mesh.getVectorVerticesSize());
							Vertex vert3;
							vert3.position = glm::vec4(position[v[3][0]-1], 1);
							vert3.texcoord = texcoord[v[3][1]-1];
							mesh.PushVertex(vert3);
							mesh.PushIndices(mesh.getVectorVerticesSize());
						}
					}
					else
					{
						for (int i = 0; i < 3; i++)
						{
							Vertex vert;
							iss >> v[i][0] >> trash >> v[i][1] >> trash >> v[i][2];
							vert.position = glm::vec4(position[v[i][0]-1], 1);
							vert.texcoord = texcoord[v[i][1]-1];
							vert.normal = normal[v[i][2]-1];
							mesh.PushVertex(vert);
							mesh.PushIndices(mesh.getVectorVerticesSize());
						}
						if (iss >> v[3][0])
						{
							iss >> trash >> v[3][1] >> trash >> v[3][2];
							Vertex vert1;
							vert1.position = glm::vec4(position[v[0][0]-1], 1);
							vert1.texcoord = texcoord[v[0][1]-1];
							vert1.normal = normal[v[0][2]-1];
							mesh.PushVertex(vert1);
							mesh.PushIndices(mesh.getVectorVerticesSize());
							Vertex vert2;
							vert2.position = glm::vec4(position[v[2][0]-1], 1);
							vert2.texcoord = texcoord[v[2][1]-1];
							vert1.normal = normal[v[2][2]-1];
							mesh.PushVertex(vert2);
							mesh.PushIndices(mesh.getVectorVerticesSize());
							Vertex vert3;
							vert3.position = glm::vec4(position[v[3][0]-1], 1);
							vert3.texcoord = texcoord[v[3][1]-1];
							vert1.normal = normal[v[3][2]-1];
							mesh.PushVertex(vert3);
							mesh.PushIndices(mesh.getVectorVerticesSize());
						}
					}

				}
			}
			mesh.setVerticesCount(verticesCount);
			std::cout << "load obj model finish path:" << path<<"  mesh vertices count=:"<<mesh.getVerticesCount()<<"  mesh triangles count=:"<<mesh.getTrianglesCount() << std::endl;
			in.close();
			mesh.BuildTriangleStream();
			return mesh;
		}

	private:
		Mesh& mesh;
	};
}
