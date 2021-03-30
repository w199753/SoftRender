#pragma once
#include<glm/vector_relational.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/matrix.hpp>
#include"../Common/Global.h"
#include"../Math/fMath.h"
#include"Transform.hpp"
#include<iostream>
namespace softRD
{
	class Camera
	{
	public:
		Camera() {}
		~Camera() {}
		//设置透视投影参数
		void SetProjectParams(const float _near, const float _far, const float _fov, const float _aspect)
		{
			fov = _fov;
			Near = _near;
			Far = _far;
			aspect = _aspect;
			float DEG2RAD = 0.0174532924f;
			float tanHalfFov = tan(fov * 0.5f * DEG2RAD);
			projectMatrix = glm::mat4x4(0);
			projectMatrix[0][0] = 1.f / (aspect * tanHalfFov);
			projectMatrix[1][1] = 1.f / (tanHalfFov);
			projectMatrix[2][2] = -(_far + _near) / (_far - _near);
			projectMatrix[2][3] = -1.f;
			projectMatrix[3][2] = -(2 * _near * _far) / (_far - _near);

			//projectMatrix = glm::perspectiveFovRH(_fov*DEG2RAD, 800.f, 600.f, _near, _far);
		}

		void SetProjectParams(const float _near, const float _far, const glm::vec2& _horizontal, const glm::vec2& _vertial)
		{
			float l = _horizontal.x, r = _horizontal.y;
			float t = _vertial.x, b = -_vertial.y;
			float n = _near, f = _far;
			projectMatrix = glm::mat4x4(1);
			projectMatrix[0][0] = 2.f / (r - l);
			projectMatrix[1][1] = 2.f / (t - b);
			projectMatrix[2][2] = 2.f / (n - f);
			projectMatrix[3][0] = -(l + r) * .5f;
			projectMatrix[3][1] = -(b + t) * .5f;
			projectMatrix[3][2] = -(n + f) * .5f;
		}
		//设置视口参数
		void SetViewportParams(int _x, int _y, int _width, int _height)
		{
			viewportMatrix[0][0] = _width * .5f;
			viewportMatrix[3][0] = _x + (_width * .5f);
			viewportMatrix[1][1] = _height * .5f;
			viewportMatrix[3][1] = _y + (_height * .5f);
			//for (size_t i = 0; i < 4; i++)
			//{
			//	for (size_t j = 0; j < 4; j++)
			//	{
			//		std::cout << viewportMatrix[i][j] << " ";
			//	}
			//	std::cout << std::endl;
			//}
			//viewportMatrix = glm::transpose(viewportMatrix);
		}
		//设置视图变换所需参数
		void SetViewParams()
		{

		}
		//设置变换参数
		void SetTransformParam(const glm::vec3& pos, const glm::vec3& lookat, const glm::vec3& _eulerAngle)
		{
			position = pos;
			forward = glm::normalize(lookat- pos);  //规定相机看向z的负半轴
			//std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
			right = glm::normalize(glm::cross(forward, worldUpDir));
			up = glm::normalize(glm::cross(right, forward));
			viewMatrix[0][0] = right.x;
			viewMatrix[1][0] = right.y;
			viewMatrix[2][0] = right.z;
			viewMatrix[0][1] = up.x;
			viewMatrix[1][1] = up.y;
			viewMatrix[2][1] = up.z;
			viewMatrix[0][2] = -forward.x;
			viewMatrix[1][2] = -forward.y;
			viewMatrix[2][2] = -forward.z;
			viewMatrix[3][0] = -glm::dot(position, right);
			viewMatrix[3][1] = -glm::dot(position, up);
			viewMatrix[3][2] = glm::dot(position, forward);

			eulerAngle = fMath::Dir2Euler(forward);
			pitch = eulerAngle.x;
			yaw = eulerAngle.y;
			std::cout << eulerAngle.x << " " << eulerAngle.y << " " << eulerAngle.z << " " << std::endl;
			//std::cout << pitch << " " << yaw << std::endl;
			//for (size_t i = 0; i < 4; i++)
			//{
			//	for (size_t j = 0; j < 4; j++)
			//	{
			//		std::cout << viewMatrix[i][j] << " ";
			//	}
			//	std::cout << std::endl;
			//}
			//viewMatrix = glm::lookAtRH(pos, glm::vec3(0), worldUpDir);
			//viewMatrix = glm::transpose(viewMatrix);
		}

		inline void MoveHorizontal(const float x)
		{
			position += x*this->right;
			updateMatrix();
		}
		inline void MoveVertical(const float y)
		{
			position += y * this->up;
			updateMatrix();
		}
		inline void MoveForward(const float z)
		{
			position += z * this->forward;
			updateMatrix();
		}

		void updateMatrix()
		{
			glm::vec3 _front = fMath::Euler2Dir(glm::vec3(pitch, yaw, 0.0f));
			forward = glm::normalize(_front);  //规定相机看向z的负半轴
			right = glm::normalize(glm::cross(forward, worldUpDir));
			up = glm::normalize(glm::cross(right, forward));
			viewMatrix[0][0] = right.x;
			viewMatrix[1][0] = right.y;
			viewMatrix[2][0] = right.z;
			viewMatrix[0][1] = up.x;
			viewMatrix[1][1] = up.y;
			viewMatrix[2][1] = up.z;
			viewMatrix[0][2] = -forward.x;
			viewMatrix[1][2] = -forward.y;
			viewMatrix[2][2] = -forward.z;
			viewMatrix[3][0] = -glm::dot(position, right);
			viewMatrix[3][1] = -glm::dot(position, up);
			viewMatrix[3][2] = glm::dot(position, forward);
			eulerAngle = fMath::Dir2Euler(forward);

			pitch = eulerAngle.x;
			yaw = eulerAngle.y<0?eulerAngle.y+360:eulerAngle.y;
			//std::cout << pitch << " " << yaw << " " << std::endl;
			//std::cout << eulerAngle.x << " " << eulerAngle.y << " " << eulerAngle.z << " " << std::endl;
			//std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
			//pitch = eulerAngle.x;
			//yaw = eulerAngle.y;
		}

		void SetRotate(const glm::vec3 angle)
		{
			pitch += angle.x;
			if (pitch > 89.0)
				pitch = 89.0;
			if (pitch < -89.0)
				pitch = -89.0;
			yaw += angle.y;
			if (yaw > 360)
				yaw = 0;
			if (yaw < 0)
				yaw = 360;
			updateMatrix();
		}

		glm::mat4x4 viewMatrix = glm::mat4x4(1);
		glm::mat4x4 projectMatrix = glm::mat4x4(0);
		glm::mat4x4 viewportMatrix = glm::mat4x4(1);
		glm::vec3 up = glm::vec3(0), right = glm::vec3(0), forward = glm::vec3(0), position = glm::vec3(0);
		glm::vec3 eulerAngle = glm::vec3(0);

		//透视投影所需参数
		float Near, Far, fov, aspect;    //这里fov为纵向，aspect为宽高比
		//相机viewportRect参数
		float X, Y, W, H;
private:
		glm::vec3 const worldUpDir = glm::vec3(0, 1, 0);

		float pitch;
		float yaw;




	};
}
