#pragma once
#include<iostream>
#include<vector>
#include<functional>
#include"../Common/Global.h"
#include"../Render/FrameBuffer.h"
#include"../Math/fMath.h"
#include "../Shade/V2f.h"
namespace softRD
{
	enum class RasterType
	{
		Fill = 1,
		Line = 2,
	};
	inline RasterType operator |(RasterType a, RasterType b)
	{
		return RasterType((int)a | (int)b);
	}
	inline RasterType operator &(RasterType a, RasterType b)
	{
		return RasterType((int)a & (int)b);
	}

	struct raster_res
	{
	public:
		raster_res() {}
		raster_res(int _x, int _y,const V2f& _o)
		{
			x = _x;
			y = _y;
			o = _o;
		}
		//inline void setWindowpos(float x, float y, float z, float w)
		//{
		//	//o.windowPos.x = x;
		//	//o.windowPos.y = y;
		//	//o.windowPos.z = z;
		//	//o.windowPos.w = w;
		//	o.windowPos = glm::vec4(x,y,z,w);
		//}

		//inline void setTexture(float x, float y)
		//{
		//	o.texcoord = glm::vec2(x,y);
		//	//o.texcoord.x = x;
		//	//o.texcoord.y = y;
		//}

		//inline void setWorldpos(float x, float y, float z, float w)
		//{
		//	o.worldPos = glm::vec4(x,y,z,w);
		//	//o.worldPos.x = x;
		//	//o.worldPos.y = y;
		//	//o.worldPos.z = z;
		//	//o.worldPos.w = w;
		//}

		//inline void setNormal(float x, float y, float z)
		//{
		//	o.normal = glm::vec3(x,y,z);
		//	//o.normal.x = x;
		//	//o.normal.y = y;
		//	//o.normal.z = z;
		//}

		//inline void setColor(float x, float y, float z,float w)
		//{
		//	o.color = glm::vec4(x,y,z,w);
		//	//o.color.x = x;
		//	//o.color.y = y;
		//	//o.color.z = z;
		//	//o.color.w = w;
		//}
		int x=0, y=0;
		V2f o;

	};
	class Rasterization
	{
	public:
		Rasterization() {  }
		~Rasterization() {}
		

		void SetRasterType(RasterType _type)
		{
			type = _type;
		}
		int index = 0;
		void RasterTriangle(const V2f& o1, const V2f& o2, const V2f& o3)
		{
			index = 0;
			if ((type & RasterType::Line) == RasterType::Line)
			{
				RasterLine(o1, o2);
				RasterLine(o2, o3);
				RasterLine(o3, o1);
			}
			if ((type & RasterType::Fill) == RasterType::Fill)
			{
				RasterFill(o1, o2, o3);
			}
		}

		void RasterFill(const V2f& o1, const V2f& o2, const V2f& o3)
		{
			int x1, y1, x2, y2;
			GetBoundBox(o1.windowPos, o2.windowPos, o3.windowPos, x1, y1, x2, y2);
			//cout << o1.texcoord <<"      "<< o2.texcoord << "      " << o3.texcoord << endl;
			//std::cout << x1 << "xx" << y1 << "xx" << x2 << " " << y2 << std::endl;
			x1 = fMath::Clamp(x1, 0, 800);
			x2 = fMath::Clamp(x2, 0, 800);
			y1 = fMath::Clamp(y1, 0, 600);
			y2 = fMath::Clamp(y2, 0, 600);
			for (register int i = x1; i <= x2; i++)
			{
				for (register int j = y1; j <= y2; j++)
				{
					//std::cout << i << " " << j << std::endl;
					//Global::frameBuffer->WriteColor(i, j, glm::vec4(1));
					if (InsideTriangle(i, j, o1.windowPos, o2.windowPos, o3.windowPos))
					{
						float rA = (-(i - o2.windowPos.x) * (o3.windowPos.y - o2.windowPos.y) + (j - o2.windowPos.y) * (o3.windowPos.x - o2.windowPos.x)) / (-(o1.windowPos.x - o2.windowPos.x) * (o3.windowPos.y - o2.windowPos.y) + (o1.windowPos.y - o2.windowPos.y) * (o3.windowPos.x - o2.windowPos.x));
						float rB = (-(i - o3.windowPos.x) * (o1.windowPos.y - o3.windowPos.y) + (j - o3.windowPos.y) * (o1.windowPos.x - o3.windowPos.x)) / (-(o2.windowPos.x - o3.windowPos.x) * (o1.windowPos.y - o3.windowPos.y) + (o2.windowPos.y - o3.windowPos.y) * (o1.windowPos.x - o3.windowPos.x));
						float rC = 1.0f - rA - rB;
						if (rA > 0 && rB > 0 && rC > 0)
						{
							float _z = o1.windowPos.z * rA + o2.windowPos.z * rB + o3.windowPos.z * rC;

							if (_z< Global::frameBuffer->GetDepth(i,j))
							{
								V2f o = V2f((o1.worldPos * rA + o2.worldPos * rB + o3.worldPos * rC),
									(o1.windowPos * rA + o2.windowPos * rB + o3.windowPos * rC),
									(o1.color * rA + o2.color * rB + o3.color * rC),
									(o1.normal * rA + o2.normal * rB + o3.normal * rC),
									(o1.texcoord * rA + o2.texcoord * rB + o3.texcoord * rC),
									(o1.Z * rA + o2.Z * rB + o3.Z * rC));
								float divZ = (1.0f / o.Z);
								//std::cout << o.Z << std::endl;
								o.worldPos *= divZ;
								o.normal *= divZ;
								o.color *= divZ;
								o.texcoord *= divZ;
								Global::frameBuffer->WriteDepth(i, j, o.windowPos.z);
								resList[index].y = j;
								resList[index].x = i;
								resList[index].o = o;
								//(resList + index)->setTexture(o.texcoord.x, o.texcoord.y);
								//(resList + index)->setWindowpos(o.windowPos.x, o.windowPos.y, o.windowPos.z, o.windowPos.w);
								//(resList + index)->setColor(o.color.x, o.color.y, o.color.z, o.color.w);
								//(resList + index)->setNormal(o.normal.x, o.normal.y, o.normal.z);
								//(resList + index)->setWorldpos(o.worldPos.x, o.worldPos.y, o.worldPos.z, o.worldPos.w);

								//resList[index].y =j;
								//resList[index].x = i; //= (raster_res(i, j, o));
								//resList[index].o = o;
								//resList[index].setTexture(o.texcoord.x, o.texcoord.y);
								//resList[index].setWindowpos(o.windowPos.x, o.windowPos.y, o.windowPos.z, o.windowPos.w);
								//resList[index].setColor(o.color.x, o.color.y, o.color.z, o.color.w);
								//resList[index].setNormal(o.normal.x, o.normal.y, o.normal.z);
								//resList[index].setWorldpos(o.worldPos.x, o.worldPos.y, o.worldPos.z, o.worldPos.w);
								index++;
								//resList[index] = raster_res(i, j, o);
								//resList.push_back(raster_res(i, j, o));
								//Global::frameBuffer->WriteColor(i, j, glm::vec4(1));
							}
							//Global::frameBuffer->WriteColor(i, j, glm::vec4(1));
						}
					
					}

				}
			}
		}
		void RasterLine(const V2f& from,const V2f& to)
		{
			int dx = to.windowPos.x - from.windowPos.x;
			int dy = to.windowPos.y - from.windowPos.y;
			int xDelta = 1, yDelta = 1;
			if (dx<0)
			{
				xDelta = -xDelta;
				dx = -dx;
			}
			if (dy<0)
			{
				yDelta = -yDelta;
				dy = -dy;
			}
			int currentX = from.windowPos.x;
			int currentY = from.windowPos.y;
			V2f tmp;
			//斜率小于1
			if (dy <= dx)
			{
				int P = 2 * dy - dx;
				for (int i = 0; i <= dx; ++i)
				{
					tmp = fMath::Lerp(from, to, ((float)(i) / dx));
					Global::frameBuffer->WriteColor(currentX, currentY, glm::vec4(0,0.8,0,1));
					currentX += xDelta;
					if (P <= 0)
						P += 2 * dy;
					else
					{
						currentY += yDelta;
						P += 2 * (dy - dx);
					}
				}
			}
			//斜率大于1，利用对称性画
			else
			{
				int P = 2 * dx - dy;
				for (int i = 0; i <= dy; ++i)
				{
					tmp = fMath::Lerp(from, to, ((float)(i) / dy));
					Global::frameBuffer->WriteColor(currentX, currentY, glm::vec4(0, 0.8, 0, 1));
					currentY += yDelta;
					if (P <= 0)
						P += 2 * dx;
					else
					{
						currentX += xDelta;
						P -= 2 * (dy - dx);
					}
				}
			}
		}

		inline bool InsideTriangle(int x, int y, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
		{
			glm::vec3 p(x, y, 0);
			auto a = glm::cross((p - v1), (v2 - v1));
			auto b = glm::cross((p - v2), (v3 - v2));
			auto c = glm::cross((p - v3), (v1 - v3));
			if ((a.z > 0 && b.z > 0 && c.z) || (a.z < 0 && b.z < 0 && c.z < 0))
				return true;
			return
				false;
		}

		inline void Rasterization::GetBoundBox(const glm::vec4& o1, const glm::vec4& o2, const glm::vec4& o3, int& x1, int& y1, int& x2, int& y2)
		{
			x1 = std::min(std::min(o1.x, o2.x), o3.x);
			x2 = std::max(std::max(o1.x, o2.x), o3.x);
			y1 = std::min(std::min(o1.y, o2.y), o3.y);
			y2 = std::max(std::max(o1.y, o2.y), o3.y);
		}

		raster_res resList[400000];
	private:

		RasterType type = RasterType::Fill;
	};


}
