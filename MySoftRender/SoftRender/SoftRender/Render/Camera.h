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
		//设置透视投影参数
		void SetProjectParams()
		{

		}
		//设置视口参数
		void SetViewportParams()
		{

		}
		//设置视图变换所需参数
		void SetViewParams()
		{

		}
		//设置变换参数
		void SetTransformParam()
		{

		}
	private:
		glm::vec3 worldUpDir = glm::vec3(0, 1, 0);

		//透视投影所需参数
		float near, far, fov,aspect;    //这里fov为纵向，aspect为宽高比
		//相机viewportRect参数
		float X, Y, W, H;

		glm::vec3 up, right, forward,position;
		glm::mat4x4 viewMatrix;
		glm::mat4x4 projectMatrix;
		glm::mat4x4 viewportMatrix;
	};
}
