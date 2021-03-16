#pragma once
#include"../Common/Global.h"
#include"Camera.h"
#include<glm/vector_relational.hpp>
namespace softRD
{
	class CullSystem
	{
	public:
		CullSystem() {}
		~CullSystem() {}

		bool FaceCull(bool isFront, const V2f& o1, const V2f& o2, const V2f& o3)
		{
			float neg = -1;
			if (isFront == false)
			{
				neg *= neg;
			}
			return glm::dot(glm::vec3(0,0,1), glm::cross(glm::vec3(o2.windowPos - o1.windowPos)*neg, glm::vec3(o3.windowPos - o1.windowPos))) > 0;
		}

		void FrumstumCull()
		{

		}

	private:

	};

}
