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
		bool needDepthTest = true;
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
				needDepthTest = false;
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
			//绘制图形
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
						Global::raster->RasterTriangle(o1, o2, o3,needDepthTest);
						Global::verticesCount += 3 / shader->pass.size();
						Global::triangleCount += 1 / shader->pass.size();
						//auto resList = ;
						int len = Global::raster->index;
						//std::cout << len << std::endl;
						for (register int i = 0; i < len; i++)
						{
							Global::frameBuffer->WriteColor(Global::raster->resList[i].x, Global::raster->resList[i].y, shader->pass[s].fragmentShader(Global::raster->resList[i].o));
						}
					}
				}
			}
			if (needDepthTest == true)
			{
				//for (int i = 0; i < stream.size(); i++)
				//{
				//	DrawTangent(*stream[i], glm::vec4(1, 0, 0, 1));
				//	DrawNormal(*stream[i], glm::vec4(0, 0, 1, 1));
				//}
			}

		}


		void DrawTangent(const Triangle& v1, glm::vec4 color)
		{
			Vertex v2_v0 = v1.v0();
			Vertex v2_v1 = v1.v1();
			Vertex v2_v2 = v1.v2();
			v2_v0.position += glm::normalize(shader->block.transform->scaleMatrix * glm::vec4(glm::vec3(v1.v0().tangent), 0));
			v2_v1.position += glm::normalize(shader->block.transform->scaleMatrix * glm::vec4(glm::vec3(v1.v1().tangent), 0));
			v2_v2.position += glm::normalize(shader->block.transform->scaleMatrix * glm::vec4(glm::vec3(v1.v2().tangent), 0));

			V2f o1_1 = shader->pass[0].vertexShader(v1.v0());
			V2f o1_2 = shader->pass[0].vertexShader(v1.v1());
			V2f o1_3 = shader->pass[0].vertexShader(v1.v2());
			PerspectiveDivision(o1_1);
			PerspectiveDivision(o1_2);
			PerspectiveDivision(o1_3);
			o1_1.windowPos = Global::mainCamera->viewportMatrix * o1_1.windowPos;
			o1_2.windowPos = Global::mainCamera->viewportMatrix * o1_2.windowPos;
			o1_3.windowPos = Global::mainCamera->viewportMatrix * o1_3.windowPos;

			V2f o2_1 = shader->pass[0].vertexShader(v2_v0);
			V2f o2_2 = shader->pass[0].vertexShader(v2_v1);
			V2f o2_3 = shader->pass[0].vertexShader(v2_v2);
			PerspectiveDivision(o2_1);
			PerspectiveDivision(o2_2);
			PerspectiveDivision(o2_3);
			o2_1.windowPos = Global::mainCamera->viewportMatrix * o2_1.windowPos;
			o2_2.windowPos = Global::mainCamera->viewportMatrix * o2_2.windowPos;
			o2_3.windowPos = Global::mainCamera->viewportMatrix * o2_3.windowPos;


			Global::raster->RasterLine(o1_1, o2_1, color);
			//Global::raster->RasterLine(o1_2, o2_2, color);
			//Global::raster->RasterLine(o1_3, o2_3, color);
		}

		void DrawNormal(const Triangle& v1, glm::vec4 color)
		{
			Vertex v2_v0 = v1.v0();
			Vertex v2_v1 = v1.v1();
			Vertex v2_v2 = v1.v2();
			v2_v0.position += glm::normalize(shader->block.transform->scaleMatrix * glm::vec4(glm::vec3(v1.v0().normal), 0));
			v2_v1.position += glm::normalize(shader->block.transform->scaleMatrix * glm::vec4(glm::vec3(v1.v1().normal), 0));
			v2_v2.position += glm::normalize(shader->block.transform->scaleMatrix * glm::vec4(glm::vec3(v1.v2().normal), 0));

			V2f o1_1 = shader->pass[0].vertexShader(v1.v0());
			V2f o1_2 = shader->pass[0].vertexShader(v1.v1());
			V2f o1_3 = shader->pass[0].vertexShader(v1.v2());
			PerspectiveDivision(o1_1);
			PerspectiveDivision(o1_2);
			PerspectiveDivision(o1_3);
			o1_1.windowPos = Global::mainCamera->viewportMatrix * o1_1.windowPos;
			o1_2.windowPos = Global::mainCamera->viewportMatrix * o1_2.windowPos;
			o1_3.windowPos = Global::mainCamera->viewportMatrix * o1_3.windowPos;

			V2f o2_1 = shader->pass[0].vertexShader(v2_v0);
			V2f o2_2 = shader->pass[0].vertexShader(v2_v1);
			V2f o2_3 = shader->pass[0].vertexShader(v2_v2);
			PerspectiveDivision(o2_1);
			PerspectiveDivision(o2_2);
			PerspectiveDivision(o2_3);
			o2_1.windowPos = Global::mainCamera->viewportMatrix * o2_1.windowPos;
			o2_2.windowPos = Global::mainCamera->viewportMatrix * o2_2.windowPos;
			o2_3.windowPos = Global::mainCamera->viewportMatrix * o2_3.windowPos;


			Global::raster->RasterLine(o1_1, o2_1, color);
			//Global::raster->RasterLine(o1_2, o2_2, color);
			//Global::raster->RasterLine(o1_3, o2_3, color);
		}

		void DrawBiTangent(const Triangle& v1, glm::vec4 color)
		{

		}

		std::unique_ptr<Shader> shader;
	private:
		ShadingType type = ShadingType::Phong;
		glm::vec4 color = glm::vec4(1);
		CullSystem cull;

		bool PreRasterSetting(V2f& o1, V2f& o2, V2f& o3)
		{
			//视椎体裁剪 -裁剪掉完全在视椎体外的图元
			if (cull.FrumstumCull(o1, o2, o3))
			{
				return false;
			}
			//视口剔除---（好坑啊，天空盒没有这个要gg啊）
			std::vector<V2f> clipingVertexs = cull.SutherlandHodgemanClip(o1, o2, o3);

			for (int i = 0; i < clipingVertexs.size(); i++) {
				PerspectiveDivision(clipingVertexs[i]);
				//视口映射
				clipingVertexs[i].windowPos = Global::mainCamera->viewportMatrix* clipingVertexs[i].windowPos;
			}
			//面剔除
			if (cull.FaceCull(cullfront, clipingVertexs[0], clipingVertexs[1], clipingVertexs[2]))
			{
				return false;
			}
			


			int n = clipingVertexs.size() - 3 + 1;
			for (int i = 0; i < n; i++) 
			{
				V2f f1 = clipingVertexs[0];
				V2f f2 = clipingVertexs[i + 1];
				V2f f3 = clipingVertexs[i + 2];

				for (int s = 0; s < shader->pass.size(); s++)
				{
					f1.color = f2.color = f3.color = color;
					Global::raster->RasterTriangle(f1, f2, f3, needDepthTest);
					int len = Global::raster->index;
					//std::cout << len << std::endl;
					for (register int i = 0; i < len; i++)
					{
						Global::frameBuffer->WriteColor(Global::raster->resList[i].x, Global::raster->resList[i].y, shader->pass[s].fragmentShader(Global::raster->resList[i].o));
					}
				}

			}
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
			v.tbn *= v.Z;
		}

	};

}
