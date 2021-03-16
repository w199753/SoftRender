#pragma once
#include<glm/vector_relational.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/matrix.hpp>
#include"../Common/Global.h"
#include"Transform.hpp"
#include<iostream>
namespace softRD
{
	class Camera
	{
	public:
		Camera() {}
		~Camera() {}
		//����͸��ͶӰ����
		void SetProjectParams(const float _near,const float _far,const float _fov,const float _aspect)
		{
			fov = _fov;
			Near = _near;
			Far = _far;
			aspect = _aspect;
			float DEG2RAD = 0.0174532924f;
			float tanHalfFov = tan(fov * 0.5f*DEG2RAD);

			projectMatrix[0][0] = 1.f / (aspect * tanHalfFov);
			projectMatrix[1][1] = 1.f / (tanHalfFov);
			projectMatrix[2][2] = -(_far + _near) / (_far - _near);
			projectMatrix[2][3] = -1.f;
			projectMatrix[3][2] = -(2 * _near * _far) / (_far - _near);

			//projectMatrix = glm::perspectiveFovRH(_fov*DEG2RAD, 800.f, 600.f, _near, _far);
		}
		//�����ӿڲ���
		void SetViewportParams(int _x,int _y,int _width,int _height)
		{
			viewportMatrix[0][0] = _width *.5f;
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
		//������ͼ�任�������
		void SetViewParams()
		{

		}
		//���ñ任����
		void SetTransformParam(const glm::vec3& pos,const glm::vec3& lookat,const glm::vec3& eulerAngle)
		{
			position = pos;
			forward = glm::normalize( lookat - pos);  //�涨�������z�ĸ�����
			std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
			right = glm::normalize(glm::cross(forward,worldUpDir));
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
			viewMatrix[3][0] = -glm::dot(pos, right);
			viewMatrix[3][1] = -glm::dot(pos, up);
			viewMatrix[3][2] = -glm::dot(pos, forward);
			for (size_t i = 0; i < 4; i++)
			{
				for (size_t j = 0; j < 4; j++)
				{
					std::cout << viewMatrix[i][j] << " ";
				}
				std::cout << std::endl;
			}
			viewMatrix = glm::lookAtRH(pos, lookat , worldUpDir);
			
			//viewMatrix = glm::transpose(viewMatrix);
		}

		glm::mat4x4 viewMatrix = glm::mat4x4(1);
		glm::mat4x4 projectMatrix = glm::mat4x4(0);
		glm::mat4x4 viewportMatrix = glm::mat4x4(1);
		glm::vec3 up, right, forward, position;
	private:
		glm::vec3 worldUpDir = glm::vec3(0, 1, 0);

		//͸��ͶӰ�������
		float Near, Far, fov,aspect;    //����fovΪ����aspectΪ��߱�
		//���viewportRect����
		float X, Y, W, H;
		



	};
}
