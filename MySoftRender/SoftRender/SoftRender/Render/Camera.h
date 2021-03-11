#pragma once
#include<glm/vector_relational.hpp>
#include<glm/matrix.hpp>
namespace softRD
{
	class Camera
	{
	public:
		Camera() {}
		~Camera() {}
		//����͸��ͶӰ����
		void SetProjectParams()
		{

		}
		//�����ӿڲ���
		void SetViewportParams()
		{

		}
		//������ͼ�任�������
		void SetViewParams()
		{

		}
		//���ñ任����
		void SetTransformParam()
		{

		}
	private:
		glm::vec3 worldUpDir = glm::vec3(0, 1, 0);

		//͸��ͶӰ�������
		float near, far, fov,aspect;    //����fovΪ����aspectΪ��߱�
		//���viewportRect����
		float X, Y, W, H;

		glm::vec3 up, right, forward,position;
		glm::mat4x4 viewMatrix;
		glm::mat4x4 projectMatrix;
		glm::mat4x4 viewportMatrix;
	};
}
