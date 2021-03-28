#pragma once
#include"../Common/Global.h"
#include"Camera.h"
#include<glm/vector_relational.hpp>
namespace softRD
{
	//顺序是：视锥体剔除(clipSpace) ， 视口裁剪(clipSpace) ， 面剔除(ndc)
	class CullSystem
	{
	public:
		CullSystem() {}
		~CullSystem() {}

		//-面剔除
		bool FaceCull(bool isFront, const V2f& o1, const V2f& o2, const V2f& o3)
		{
			float neg = -1;
			if (isFront == false)
			{
				neg *= neg;
			}
			return glm::dot(glm::vec3(0, 0, 1), glm::cross(glm::vec3(o2.windowPos - o1.windowPos) * neg, glm::vec3(o3.windowPos - o1.windowPos))) > 0;
		}

		//-视锥体剔除
		//-平面方程法：求得视锥体的六个面平面方程，去判断电和平面的关系
		//-另一钟： -w<x,y,z<w   near<w<far
		bool FrumstumCull(const V2f& o1, const V2f& o2, const V2f& o3)
		{
			float _near = Global::mainCamera->Near;
			float _far = Global::mainCamera->Far;
			if ((o1.windowPos.w > _far || o1.windowPos.w < _near) &&
				(o2.windowPos.w > _far || o2.windowPos.w < _near) &&
				(o3.windowPos.w > _far || o3.windowPos.w < _near))
			{
				return true;
			}
			bool flag1 = (o1.windowPos.x<-o1.windowPos.w || o1.windowPos.x>o1.windowPos.w) && (o2.windowPos.x<-o2.windowPos.w || o2.windowPos.x>o2.windowPos.w) && (o3.windowPos.x<-o3.windowPos.w || o3.windowPos.x>o3.windowPos.w);
			bool flag2 = (o1.windowPos.y<-o1.windowPos.w || o1.windowPos.y>o1.windowPos.w) && (o2.windowPos.y<-o2.windowPos.w || o2.windowPos.y>o2.windowPos.w) && (o3.windowPos.y<-o3.windowPos.w || o3.windowPos.y>o3.windowPos.w);
			bool flag3 = (o1.windowPos.z<-o1.windowPos.w || o1.windowPos.z>o1.windowPos.w) && (o2.windowPos.z<-o2.windowPos.w || o2.windowPos.z>o2.windowPos.w) && (o3.windowPos.z<-o3.windowPos.w || o3.windowPos.z>o3.windowPos.w);

			return flag1&flag2&flag3;
		}

		void SutherlandHodgemanClip()
		{

		}

	private:

	};

}
