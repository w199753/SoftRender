#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<Windows.h>
#include<string>
#include<thread>
#include<algorithm>

#include <iostream>
#include <string.h>
#include "Render/FrameBuffer.h"
#include "Render/Object.h"
#include"Shade/PhongShader.h"

using namespace std;
using namespace softRD;
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int fps = 0;
void showfps(GLFWwindow* window)
{
	while (1)
	{
		Sleep(1000);
		string txt = "FPS:" + std::to_string(fps);
		glfwSetWindowTitle(window, txt.c_str());
		fps = 0;
	}

}
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace std;

class AA
{
public:
	int aaa;
	AA() { data = make_unique<int>(); cout << "cons aa" << endl; }
	AA(const AA& a)
	{
		AA bb;
		//cout << "aa" << endl;
		
		aaa = a.aaa;
		
	}
	std::shared_ptr<int> data;
};


class BB
{
public:
	int bbb;
};

int main()
{

	//std::unique_ptr<Camera> main = ;
	Global::mainCamera = std::make_unique<Camera>();
	Global::mainCamera->SetViewportParams(0, 0, 800.f, 600.f);
	Global::mainCamera->SetTransformParam(glm::vec3(0,0,1), glm::vec3(0, 0, 1)+ glm::vec3(0, 0, -1), glm::vec3(0));
	Global::mainCamera->SetProjectParams(1.f, 100.f, 60.f, 800.f / 600.f);

	Global::frameBuffer = std::make_unique<FrameBuffer>(800, 600);

	Global::raster = std::make_unique<Rasterization>();
	Global::raster->SetRasterType(RasterType::Fill);

	PropertyBlock block;
	block.albedo = std::make_unique<Texture>("Model/textures/Albedo.png",TextureType::LDR);
	std::unique_ptr<Shader> shader = std::make_unique<PhongShader>(block);
	Material obj_material;
	obj_material.SetShader(std::move(shader));
	//Object obj("Model/cube_1.obj",obj_material);
	//obj.SetScale(0.8, 0.8, 0.3);
	//obj.SetRotate(0, 10, 0);
	//obj.SetTranslate(-0.5, -12.8, 25);
	//Object obj("Model/face.obj", obj_material);
	//obj.SetScale(1., 1., 1);
	//obj.SetRotate(0, 10, 0);
	//obj.SetTranslate(0, -0.5, -3);
	Object obj("Model/Scanner.obj",obj_material);
	obj.SetScale(0.0004, 0.0004, 0.0004);
	obj.SetRotate(0, 40, 0);
	obj.SetTranslate(0, -0.5, -0.5);

	//PropertyBlock block1;
	//block1.albedo = std::make_unique<Texture>("Model/textures/Albedo.png", TextureType::LDR);
	//std::unique_ptr<Shader> shader1 = std::make_unique<PhongShader>(block1);
	//Material obj_material1;
	//obj_material1.SetShader(std::move(shader1));
	//Object obj1("Model/Test.obj", obj_material1);
	//obj1.SetScale(1, 1, 0.9);
	//obj1.SetTranslate(0.5, 0, 0);

	

	//Test* tt = new Test();
	//change(tt,200);
	//std::cout << tt->aaa << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SoftRender", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	std::thread showFPS(showfps, window);
	showFPS.detach();
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		Global::frameBuffer->ClearBuffer(BuffersType::COLOR | BuffersType::DEPTH);

		//for (int i = 0; i < 200; i++)
		//{
		//	for (int j = 0; j < 200; j++)
		//		Global::frameBuffer->WriteColor(i,j, glm::vec4(1));
		//}
		obj.RenderObject();
		//obj1.RenderObject();
		//
		////
		////glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		////glClear(GL_COLOR_BUFFER_BIT);
		////
		glDrawPixels(Global::frameBuffer->width, Global::frameBuffer->height, GL_RGBA, GL_UNSIGNED_BYTE, Global::frameBuffer->colorBuffer.data());
		glfwSwapBuffers(window);
		glfwPollEvents();
		fps++;
	}

	//Global::mainCamera.release();
	glfwTerminate();
	return 0;
}




