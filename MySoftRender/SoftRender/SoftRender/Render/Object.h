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

		void RenderObject()
		{
			material.Render(mesh.getTriangleStream());
		}

		//Transform& getTrans()const{return std::move(trans;}

		Mesh GetMesh()const { return mesh; }
	private:
		Mesh mesh;
		Material& material;
	};

}
