#pragma once
#include<iostream>
#include<memory>
#include"Transform.hpp"
#include"Mesh.h"
#include"Material.h"
#include"ObjModelLoader.h"
namespace softRD
{
	class Object
	{
	public:
		Object(const std::string& objPath, Material& _material):material(_material)
		{
			ObjModelLoader loader(mesh);
			loader.LoadModelMesh(objPath);
		}

		Object(int obj, Material& _material) :material(_material)
		{
			ObjModelLoader loader(mesh);
			glm::vec3 center(0);
			float radius = 0.5;
			if (obj == 1) //panel
			{
				loader.CreatePanel(center + glm::vec3(-radius, radius, radius),
					center + glm::vec3(-radius, -radius, radius),
					center + glm::vec3(radius, -radius, radius),
					center + glm::vec3(radius, radius, radius),
					glm::vec3(0, 0, 1));
			}
			else if (obj == 2)
			{
				loader.CreateBox(center, radius);
			}
		}

		Object& operator = (const Object& obj)
		{
			//mesh = obj.mesh;
			//material = obj.material;
		}
		~Object() {}

		void SetTranslate(const float x, const float y, const float z)
		{
			material.shader->block.transform->SetTranslate(x, y, z);
		}
		
		void SetScale(const float x, const float y, const float z)
		{
			material.shader->block.transform->SetScale(x, y, z);
		}
		
		void SetRotate(const float x, const float y, const float z)
		{
			material.shader->block.transform->SetRotate(x, y, z);
		}

		inline void RenderObject()
		{
			material.Render(mesh.getTriangleStream());
		}

		//Transform& getTrans()const{return std::move(trans;}

		inline const Mesh& GetMesh()const { return mesh; }
	
		Mesh mesh;
		Material& material;
	private:
	};

}
