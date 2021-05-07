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
			return glm::dot(glm::vec3(0, 0, 1), glm::cross(glm::vec3(o2.windowPos - o1.windowPos) * neg, glm::vec3(o3.windowPos - o1.windowPos))) >= 0;
		}

		//-视锥体剔除
		//-平面方程法：求得视锥体的六个面平面方程，去判断电和平面的关系
		//-另一钟： -w<x,y,z<w   near<w<far
		bool FrumstumCull(const V2f& o1, const V2f& o2, const V2f& o3)
		{
			float _near = Global::mainCamera->Near;
			float _far = Global::mainCamera->Far;
			//std::cout << o1.windowPos.w << " " << o2.windowPos.w <<" "<< o3.windowPos.w<< std::endl;
			if (o1.windowPos.w < _near && o2.windowPos.w < _near && o3.windowPos.w < _near)
				return true;
			if (o1.windowPos.w >_far && o2.windowPos.w > _far && o3.windowPos.w > _far)
				return true;

			if (fabs(o1.windowPos.x) <= fabs(o1.windowPos.w) || fabs(o1.windowPos.y) <= fabs(o1.windowPos.w) || fabs(o1.windowPos.z) <= fabs(o1.windowPos.w))
				return false;
			if (fabs(o2.windowPos.x) <= fabs(o2.windowPos.w) || fabs(o2.windowPos.y) <= fabs(o2.windowPos.w) || fabs(o2.windowPos.z) <= fabs(o2.windowPos.w))
				return false;
			if (fabs(o3.windowPos.x) <= fabs(o3.windowPos.w) || fabs(o3.windowPos.y) <= fabs(o3.windowPos.w) || fabs(o3.windowPos.z) <= fabs(o3.windowPos.w))
				return false;
			return true;

			//bool flag1 = (o1.windowPos.x<-o1.windowPos.w || o1.windowPos.x>o1.windowPos.w) && (o2.windowPos.x<-o2.windowPos.w || o2.windowPos.x>o2.windowPos.w) && (o3.windowPos.x<-o3.windowPos.w || o3.windowPos.x>o3.windowPos.w);
			//bool flag2 = (o1.windowPos.y<-o1.windowPos.w || o1.windowPos.y>o1.windowPos.w) && (o2.windowPos.y<-o2.windowPos.w || o2.windowPos.y>o2.windowPos.w) && (o3.windowPos.y<-o3.windowPos.w || o3.windowPos.y>o3.windowPos.w);
			//bool flag3 = (o1.windowPos.z<-o1.windowPos.w || o1.windowPos.z>o1.windowPos.w) && (o2.windowPos.z<-o2.windowPos.w || o2.windowPos.z>o2.windowPos.w) && (o3.windowPos.z<-o3.windowPos.w || o3.windowPos.z>o3.windowPos.w);
			//
			//return flag1&flag2&flag3;
		}

		const std::vector<glm::vec4> ViewPlanes = {
			//Near
			glm::vec4(0,0,1,1),
			//far
			glm::vec4(0,0,-1,1),
			//left
			glm::vec4(1,0,0,1),
			//top
			glm::vec4(0,1,0,1),
			//right
			glm::vec4(-1,0,0,1),
			//bottom 
			glm::vec4(0,-1,0,1)
		};

		bool Inside(const glm::vec4& line, const glm::vec4& p) {
			return line.x * p.x + line.y * p.y + line.z * p.z + line.w * p.w > -(1e-5f);
		}

		bool AllVertexsInside(const std::vector<V2f> v) {
			for (int i = 0; i < v.size(); i++) {
				if (fabs(v[i].windowPos.x) > fabs(v[i].windowPos.w))
					return false;
				if (fabs(v[i].windowPos.y) > fabs(v[i].windowPos.w))
					return false;
				if (fabs(v[i].windowPos.z) > fabs(v[i].windowPos.w))
					return false;
			}
			return true;
		}

		//交点，通过端点插值
		V2f Intersect(const V2f& v1, const V2f& v2, const glm::vec4& line) {
			float da = v1.windowPos.x * line.x + v1.windowPos.y * line.y + v1.windowPos.z * line.z + v1.windowPos.w * line.w;
			float db = v2.windowPos.x * line.x + v2.windowPos.y * line.y + v2.windowPos.z * line.z + v2.windowPos.w * line.w;

			float weight = da / (da - db);
			return fMath::Lerp(v1, v2, weight);
		}

		std::vector<V2f> SutherlandHodgemanClip(const V2f& v1, const V2f& v2, const V2f& v3)
		{
			std::vector<V2f> output = { v1,v2,v3 };
			if (AllVertexsInside(output)) {
				return output;
			}
			for (int i = 0; i < ViewPlanes.size(); i++) {
				std::vector<V2f> input(output);
				output.clear();
				for (int j = 0; j < input.size(); j++) {
					V2f current = input[j];
					V2f last = input[(j + input.size() - 1) % input.size()];
					if (Inside(ViewPlanes[i], current.windowPos)) {
						if (!Inside(ViewPlanes[i], last.windowPos)) {
							V2f intersecting = Intersect(last, current, ViewPlanes[i]);
							output.push_back(intersecting);
						}
						output.push_back(current);
					}
					else if (Inside(ViewPlanes[i], last.windowPos)) {
						V2f intersecting = Intersect(last, current, ViewPlanes[i]);
						output.push_back(intersecting);
					}
				}
			}
			return output;
		}

	private:

	};

}
