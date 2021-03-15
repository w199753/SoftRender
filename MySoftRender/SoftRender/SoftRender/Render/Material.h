#pragma once
#include "Texture.h"
#include "../Shade/Shader.h"
#include"PropertyBlock.h"
#include"Rasterization.h"
#include"Camera.h"
#include"Mesh.h"
#include<memory>
#include<iostream>
using namespace std;
namespace softRD
{
	enum class ShadingType
	{
		Flat,
		Gourand,
		Phong,
		PBR,
		Skybox,
	};
	class Material
	{
	public:
		int sortingOrder = 0;
		Material() {}
		Material(ShadingType _type,int _sortingOrder = 0)
		{
			type = _type;
			sortingOrder = _sortingOrder;
			switch (_type)
			{
			case softRD::ShadingType::Flat:
				break;
			case softRD::ShadingType::Gourand:
				break;
			case softRD::ShadingType::Phong:
				break;
			case softRD::ShadingType::PBR:
				break;
			case softRD::ShadingType::Skybox:
				break;
			default:
				break;
			}
			
		}
		~Material() {}

		void SetTransform()
		{

		}

		void SetShader(std::unique_ptr<Shader> _shader)
		{
			shader = std::move(_shader);
		}

		void Render(std::vector<Triangle*> stream)
		{
			for (int i = 0; i < stream.size(); i++)
			{
				//std::cout << stream[i]->v0().position.x << " " << stream[i]->v0().position.y << " " << stream[i]->v0().position.z << std::endl;
				//std::cout << stream[i]->v1().position.x << " " << stream[i]->v1().position.y << " " << stream[i]->v1().position.z << std::endl;
				//std::cout << stream[i]->v2().position.x << " " << stream[i]->v2().position.y << " " << stream[i]->v2().position.z << std::endl;
				V2f o1 = shader->VertexShader(stream[i]->v0());
				V2f o2 = shader->VertexShader(stream[i]->v1());
				V2f o3 = shader->VertexShader(stream[i]->v2());
				//cout <<"xxxxxx"<< o1.windowPos.x << " " << o1.windowPos.y << " " << o1.windowPos.z << endl;
				if (PreRasterSetting(o1,o2,o3))
				{
					//cout << "***********" << o1.windowPos.x << " " << o1.windowPos.y << " " << o1.windowPos.z << endl;
					//cout <<"vvvvvvvvv"<< o1.windowPos.x << " " << o1.windowPos.y << " " << o1.windowPos.z << endl;
					Global::raster->RasterTriangle(o1, o2, o3);
				}
			}
		}
		std::unique_ptr<Shader> shader;
	private:
		ShadingType type = ShadingType::Phong;


		bool PreRasterSetting(V2f& o1,V2f& o2,V2f& o3)
		{
			PerspectiveDivision(o1);
			PerspectiveDivision(o2);
			PerspectiveDivision(o3);

			//视口变换
			o1.windowPos = Global::mainCamera->viewportMatrix * o1.windowPos;
			o2.windowPos = Global::mainCamera->viewportMatrix * o2.windowPos;
			o3.windowPos = Global::mainCamera->viewportMatrix * o3.windowPos;
			return true;
		}

		void PerspectiveDivision(V2f& v) {
			v.windowPos /= v.windowPos.w;
			v.windowPos.w = 1.0f;
			// OpenGL的Z-Buffer是 [0,1]，而透视除法之后Z除了深度测试已经没用了
			v.windowPos.z = (v.windowPos.z + 1.0) * 0.5;
		}
		//PropertyBlock block;
	};

}
