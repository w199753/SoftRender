#pragma once
#include<glm/vector_relational.hpp>
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
			forward = glm::normalize(lookat - pos);
			right = glm::normalize(glm::cross(forward,worldUpDir ));
			up = glm::normalize(glm::cross(right,forward));
			viewMatrix = glm::mat4x4(right.x,up.x,-forward.x,0.f,
									right.y,up.y,-forward.y,0.f,
									right.z,up.z,-forward.z,0.f,
									-glm::dot(pos,right),-glm::dot(pos,up),-glm::dot(pos,forward),1.f);
			for (size_t i = 0; i < 4; i++)
			{
				for (size_t j = 0; j < 4; j++)
				{
					std::cout << viewMatrix[i][j] << " ";
				}
				std::cout << std::endl;
			}
			//viewMatrix = glm::transpose(viewMatrix);
		}

		glm::mat4x4 viewMatrix = glm::mat4x4(1);
		glm::mat4x4 projectMatrix = glm::mat4x4(0);
		glm::mat4x4 viewportMatrix = glm::mat4x4(1);
	private:
		glm::vec3 worldUpDir = glm::vec3(0, 1, 0);

		//͸��ͶӰ�������
		float Near, Far, fov,aspect;    //����fovΪ����aspectΪ��߱�
		//���viewportRect����
		float X, Y, W, H;
		
		glm::vec3 up, right, forward,position;


	};
}
