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
	class Rasterization
	{
	public:
		Rasterization() {}
		~Rasterization() {}

		void SetRasterType(RasterType _type)
		{
			type = _type;
		}

		void RasterTriangle(const V2f& o1, const V2f& o2, const V2f& o3)
		{
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
			for (int i = x1; i <= x2; i++)
			{
				for (int j = y1; j <= y2; j++)
				{
					//std::cout << i << " " << j << std::endl;
					//Global::frameBuffer->WriteColor(i, j, glm::vec4(1));
					if (InsideTriangle(i, j, o1.windowPos, o2.windowPos, o3.windowPos))
					{
						glm::vec3 centric = fMath::barycentric(o1.windowPos, o2.windowPos, o3.windowPos, glm::vec4(i, j, 0, 0));
						//cout << vec2(i, j) << endl;
						if (centric.x > 0 && centric.y > 0 && centric.z > 0)
						{
							V2f o;
							//重心坐标可能会变，需要明天找解决办法(好像是变换回去，在进行一次中心坐标插值,还是不太懂。。。。。
							o = o1 * centric.x + o2 * centric.y + o3 * centric.z;
							//o = o * (1/o.Z);
							//float divZ = (1.0f / o.Z);
							////cout << o.Z << endl;
							//o.worldPos *= divZ;
							//o.normal *= divZ;
							//o.color *= divZ;
							//o.texcoord *= divZ;
							////if (o.windowPos.z<Global::frontBuufer->GetDepth(i, j))
							//{
							//	//Global::frontBuufer->WriteDepth(i, j, o.windowPos.z);
							//	Global::frontBuufer->WritePoint(i, j, shader->FragmentShader(o));
							//}
							Global::frameBuffer->WriteColor(i, j, glm::vec4(1));
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
					Global::frameBuffer->WriteColor(currentX, currentY, glm::vec4(1));
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
					Global::frameBuffer->WriteColor(currentX, currentY, glm::vec4(1));
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

		bool InsideTriangle(int x, int y, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
		{
			glm::vec3 p(x, y, 0);
			auto a = glm::cross((p - v1), (v2 - v1));
			auto b = glm::cross((p - v2), (v3 - v2));
			auto c = glm::cross((p - v2), (v1 - v3));
			if ((a.z > 0 && b.z > 0 && c.z) || (a.z < 0 && b.z < 0 && c.z < 0))
				return true;
			return
				false;
		}

		void Rasterization::GetBoundBox(const glm::vec4& o1, const glm::vec4& o2, const glm::vec4& o3, int& x1, int& y1, int& x2, int& y2)
		{
			//cout<< (min(o1.x, o2.x), o3.x)<<endl;
			x1 = std::min(std::min(o1.x, o2.x), o3.x);
			x2 = std::max(std::max(o1.x, o2.x), o3.x);
			y1 = std::min(std::min(o1.y, o2.y), o3.y);
			y2 = std::max(std::max(o1.y, o2.y), o3.y);
			//cout <<o1.x<<" "<<o2.x<<" "<< x1 << " " << x2 << endl;
		}

	private:
		std::vector < std::function<int(int)>> hh;
		RasterType type = RasterType::Fill;
	};
}
