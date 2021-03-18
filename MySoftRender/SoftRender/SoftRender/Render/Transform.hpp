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
			float a = x * DEG2RAD;
			float b = y * DEG2RAD;
			float c = z * DEG2RAD;
			rotateMatrix[0][0] = cos(b)*cos(c);
			rotateMatrix[0][1] = -sin(c)*cos(a)+cos(c)*sin(b)*sin(a);
			rotateMatrix[0][2] = sin(c)*sin(a)+cos(c)*sin(b)*cos(a);
			rotateMatrix[1][0] = cos(b)*sin(c);
			rotateMatrix[1][1] = cos(c)*cos(a)+sin(c)*sin(b)*sin(a);
			rotateMatrix[1][2] = -cos(c)*sin(a)+sin(c)*sin(b)*cos(a);
			rotateMatrix[2][0] = -sin(b);
			rotateMatrix[2][1] = cos(b)*sin(a);
			rotateMatrix[2][2] = cos(b)*cos(a);
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
