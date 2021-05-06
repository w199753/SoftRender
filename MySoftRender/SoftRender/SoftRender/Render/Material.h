#pragma once
#include "Texture.h"
#include "../Shade/Shader.h"
#include"PropertyBlock.h"
#include"Rasterization.h"
#include"CullSystem.h"
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
		bool cullfront = false;
		Material() {}

		Material(ShadingType _type, int _sortingOrder = 0)
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
				cullfront = true;
				break;
			default:
				break;
			}

		}
		~Material() {}

		void SetTransform()
		{

		}

		void SetColor(const glm::vec4& col)
		{
			color = col;
		}

		void SetShader(std::unique_ptr<Shader>& _shader)
		{
			shader = std::move(_shader);
			//shader->AddPass();
			//shader->AddPass(shader->VertexShader(),)
		}

		void Render(const std::vector<Triangle*>& stream)
		{
			Global::verticesCount = 0;
			Global::triangleCount = 0;
			for (int i = 0; i < stream.size(); i++)
			{
				for (int s = 0; s < shader->pass.size(); s++)
				{
					//continue;
					V2f o1 = shader->pass[s].vertexShader(stream[i]->v2());
					
					V2f o2 = shader->pass[s].vertexShader(stream[i]->v1());
					V2f o3 = shader->pass[s].vertexShader(stream[i]->v0());
					//cout <<"xxxxxx"<< o1.windowPos.x << " " << o1.windowPos.y << " " << o1.windowPos.z << endl;
					
					if (PreRasterSetting(o1, o2, o3))
					{
						//cout << "***********" << o1.windowPos.x << " " << o1.windowPos.y << " " << o1.windowPos.z << endl;
						//cout <<"vvvvvvvvv"<< o1.windowPos.x << " " << o1.windowPos.y << " " << o1.windowPos.z << endl;
						o1.color = o2.color = o3.color = color;
						Global::raster->RasterTriangle(o1, o2, o3);
						Global::verticesCount += 3 / shader->pass.size();
						Global::triangleCount += 1 / shader->pass.size();
						//auto resList = ;
						int len = Global::raster->index;
						//std::cout << len << std::endl;
						for (register int i = 0; i < len; i++)
						{
							Global::frameBuffer->WriteColor(Global::raster->resList[i].x, Global::raster->resList[i].y, shader->pass[s].fragmentShader(Global::raster->resList[i].o));
							//Global::frameBuffer->WriteColor(resList[i].x, resList[i].y, shader->FragmentShader(resList[i].o));

						//Global::frameBuffer->WriteColor(resList[i].x, resList[i].y, glm::vec4(1));
						}
					}
				}
			}
		}
		std::unique_ptr<Shader> shader;
	private:
		ShadingType type = ShadingType::Phong;
		glm::vec4 color = glm::vec4(1);
		CullSystem cull;

		bool PreRasterSetting(V2f& o1, V2f& o2, V2f& o3)
		{

			//cout << o1.windowPos.x << " " << o1.windowPos.y << " " << o1.windowPos.z << " " << o1.windowPos.w << endl;
			if (cull.FrumstumCull(o1, o2, o3))
			{
				return false;
			}

			PerspectiveDivision(o1);
			PerspectiveDivision(o2);
			PerspectiveDivision(o3);

			if (cull.FaceCull(cullfront, o1, o2, o3))
			{
				return false;
			}

			//视口变换
			o1.windowPos = Global::mainCamera->viewportMatrix * o1.windowPos;
			o2.windowPos = Global::mainCamera->viewportMatrix * o2.windowPos;
			o3.windowPos = Global::mainCamera->viewportMatrix * o3.windowPos;
			return true;
		}

		void PerspectiveDivision(V2f& v) {
			//cout << v.windowPos.z << " " << v.windowPos.w << endl;
			v.Z = 1.f / v.windowPos.w;
			v.windowPos /= v.windowPos.w;
			v.windowPos.w = 1.0f;
			// OpenGL的Z-Buffer是 [0,1]，而透视除法之后Z除了深度测试已经没用了
			v.windowPos.z = (v.windowPos.z + 1.0) * 0.5;

			//--透视矫正插值:使用w还是使用z关系不大，因为可以推出用1/z做矫正，z和w为线性关系，后来业界统一为用w
			v.normal *= v.Z;
			//v.color *= v.Z;
			v.texcoord *= v.Z;
			v.worldPos *= v.Z;
		}

	};

}
