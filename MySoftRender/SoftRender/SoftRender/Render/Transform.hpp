#pragma once
#include <glm/matrix.hpp>
namespace softRD
{
	class Transform
	{
	public:
		Transform() {}
		~Transform() {}

		void SetTranslate(const float x,const float y,const float z)
		{
			translateMatrix[3][0] = x;
			translateMatrix[3][1] = y;
			translateMatrix[3][2] = z;
		}

		void SetRotate(const float x, const float y, const float z)
		{
			float DEG2RAD = 0.0174532924f;

		}
		void SetScale(const float x, const float y, const float z)
		{
			scaleMatrix[0][0] = x;
			scaleMatrix[1][1] = y;
			scaleMatrix[2][2] = z;
		}

		void SetScale(const float p)
		{
			scaleMatrix[0][0] = p;
			scaleMatrix[1][1] = p;
			scaleMatrix[2][2] = p;
		}

		glm::mat4x4 translateMatrix = glm::mat4x4(1);
		glm::mat4x4 rotateMatrix = glm::mat4x4(1);
		glm::mat4x4 scaleMatrix = glm::mat4x4(1);
	private:
		//glm::mat4x4 viewMatrix = glm::mat4x4(1);
		//glm::mat4x4 projectMatrix = glm::mat4x4(0);
		//glm::mat4x4 viewportMatrix = glm::mat4x4(1);

	};
}
