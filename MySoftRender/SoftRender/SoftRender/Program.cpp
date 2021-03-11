#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<Windows.h>
#include<string>
#include<thread>
#include<algorithm>
#include"stb_image.h"
#include <iostream>
#include <string.h>
#include "Render/FrameBuffer.h"
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

int main()
{

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

	FrameBuffer fb(800, 600);

	std::thread showFPS(showfps, window);
	showFPS.detach();
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		fb.ClearBuffer(BuffersType::COLOR | BuffersType::DEPTH);

		//for (int i = 0; i < 100; i++)
		//{
		//	for (int j = 0; j < 100; j++)
		//	fb.WriteColor(i,j, glm::vec4(1));
		//}
		
		//
		////
		////glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		////glClear(GL_COLOR_BUFFER_BIT);
		////
		glDrawPixels(fb.width, fb.height, GL_RGBA, GL_UNSIGNED_BYTE, fb.colorBuffer.data());
		glfwSwapBuffers(window);
		glfwPollEvents();
		fps++;
	}


	glfwTerminate();
	return 0;
}




