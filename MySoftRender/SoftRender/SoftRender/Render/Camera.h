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

		void SetCameraParams()
		{

		}
	private:
		glm::vec3 worldUpDir = glm::vec3(0, 1, 0);

		//透视投影所需参数
		float near, far, fov,aspect;    //这里fov为纵向，aspect为宽高比
		//相机viewportRect参数
		float X, Y, W, H;

		glm::vec3 up, right, forward;
		glm::mat4x4 viewMatrix;
		glm::mat4x4 projectMatrix;
		glm::mat4x4 viewportMatrix;
	};
}
