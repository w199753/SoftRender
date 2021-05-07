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
#include"Shade/UnlitShader.h"
#include"Shade/SkyBoxPreCompute.h"
#include"Shade/SkyboxShader.h"
#include"Light/Light.h"

#include "stb_image.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif // !STB_IMAGE_WRITE_IMPLEMENTATION



using namespace std;
using namespace softRD;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 pos = glm::vec3(0, 0, 1.4);
glm::vec3 look = glm::vec3(0, 0, -1);
glm::vec3 rot = glm::vec3(0, 0, 0);
bool addDelta = false;
float delta = 0.5f;
bool isDragView = false;
double startx = 0;
double starty = 0;
int fps = 0;
int vertCount = 0;
int traCount = 0;

bool firstMouse = true;
double lastX = 0;
double lastY = 0;

//button:0左键，1右键，2中
//Action：1按下 0释放
//Mode:没用到不知道
static void mouse_btn_callback(GLFWwindow* pWindow, int Button, int Action, int Mode)
{
	if (Button == 1)
	{
		isDragView = Action == 1 ? true : false;
		if (isDragView)
		{
			//https://blog.csdn.net/qq_40239482/article/details/105834521
			glfwGetCursorPos(pWindow, &startx, &starty);
			//starty = SCR_HEIGHT - starty;
			//startx = startx - SCR_WIDTH;
			//starty = starty - SCR_HEIGHT;
			//startx
			lastX = startx;
			lastY = SCR_HEIGHT - starty;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	ypos = SCR_HEIGHT - ypos;
	//xpos = xpos - SCR_WIDTH;
	//ypos = ypos - SCR_HEIGHT;
	if (isDragView)
	{
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;//灵敏度
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		Global::mainCamera->SetRotate(glm::vec3(-yoffset, -xoffset, 0));
	}
}

void scroll_callback(GLFWwindow* window, double xoffeset, double yoffset)
{
	pos.z -= yoffset * 0.5;
	Global::mainCamera->MoveForward(yoffset * 0.5f);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		addDelta = true;
		Global::mainCamera->MoveHorizontal(-0.01 * delta);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		addDelta = true;
		pos.x += 0.02 * delta;
		Global::mainCamera->MoveHorizontal(0.01 * delta);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		addDelta = true;
		pos.z -= 0.02 * delta;
		Global::mainCamera->MoveForward(0.01 * delta);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		addDelta = true;
		pos.z += 0.02 * delta;
		Global::mainCamera->MoveForward(-0.01 * delta);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		addDelta = true;
		pos.y -= 0.02 * delta;
		Global::mainCamera->MoveVertical(-0.01 * delta);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		addDelta = true;
		pos.y += 0.02 * delta;
		Global::mainCamera->MoveVertical(0.01 * delta);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
	{
		addDelta = false;
		delta = 0.5;
	}
	if (addDelta)
	{
		delta += 0.05;
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


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
	BB() { cout << "BB" << endl; }
	BB(const BB& b)
	{
		{ cout << "BB" << endl; }
	}
	~BB() { cout << "NOBB" << endl; }
};

template<typename T>
void creataMaterial(Material& mat)
{
	PropertyBlock* block = new PropertyBlock;    //没错，懒得释放指针了
	std::unique_ptr<Shader> shader = std::make_unique<T>(*block);
	mat.SetShader(std::move(shader));
}

void PreCompute()
{

}


int main()
{
	PreCompute();
	vector<BB> bbb;
	bbb.reserve(3);
	bbb.emplace_back();
	bbb.emplace_back();
	bbb.emplace_back();

	//bbb.push_back(BB());
	//bbb.push_back(BB());
	//bbb.push_back(BB());

	Global::mainCamera = std::make_unique<Camera>();
	Global::mainCamera->SetViewportParams(0, 0, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT));
	Global::mainCamera->SetTransformParam(pos, glm::vec3(0), glm::vec3(0));
	Global::mainCamera->SetProjectParams(0.4f, 1000.f, 60.f, static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT));

	Global::frameBuffer = std::make_unique<FrameBuffer>(SCR_WIDTH, SCR_HEIGHT);

	Global::raster = std::make_unique<Rasterization>();
	Global::raster->SetRasterType(RasterType::Fill);


	auto txt = std::make_shared<Texture>("Model/textures/Albedo.png", TextureType::LDR, true);
	txt->filterType = FilterType::Bilinear;
	Material phong_material;
	Material phong_material2;
	creataMaterial<PhongShader>(phong_material);
	creataMaterial<PhongShader>(phong_material2);
	phong_material.shader->block.albedo = txt;
	phong_material2.shader->block.albedo = txt;

	Material unlit_material1;
	creataMaterial<UnlitShader>(unlit_material1);

	Material unlit_material2;
	creataMaterial<UnlitShader>(unlit_material2);

	Material unlit_material3;
	creataMaterial<UnlitShader>(unlit_material3);

	//Object obj("Model/cube_2.obj", unlit_material);
	//obj.SetScale(0.1, 0.1, 0.1);
	//obj.SetRotate(0, 10, 0);
	//obj.SetTranslate(0, 0, 0);

	auto obj_light_1 = std::make_unique<Object>("Model/cube_2.obj", unlit_material1);
	auto obj_light_2 = std::make_unique<Object>("Model/cube_2.obj", unlit_material2);
	auto obj_light_3 = std::make_unique<Object>("Model/face.obj", unlit_material3);

	Object obj1("Model/face.obj", phong_material2);
	obj1.SetScale(0.6, 0.6, 0.6);
	obj1.SetRotate(0, 10, 0);
	obj1.SetTranslate(0, -0.5, 0);
	Object obj("Model/Scanner.obj", phong_material);
	obj.SetScale(0.0004, 0.0004, 0.0004);
	obj.SetRotate(0, 90, 0);
	obj.SetTranslate(0, -0.5, 0.0+0.45);

	//PropertyBlock block1;
	//block1.albedo = std::make_unique<Texture>("Model/textures/Albedo.png", TextureType::LDR);
	//std::unique_ptr<Shader> shader1 = std::make_unique<PhongShader>(block1);
	//Material obj_material1;
	//obj_material1.SetShader(std::move(shader1));
	//Object obj1("Model/Test.obj", obj_material1);
	//obj1.SetScale(1, 1, 0.9);
	//obj1.SetTranslate(0.5, 0, 0);

	//obj.RenderObject();

	//-----------------------------------------------------------------------------创建灯光
	auto pointLight1 = make_unique<PointLight>(glm::vec3(0, 0.0, 0.74), 2.8, glm::vec4(1, 0.8, 1, 1),std::move(obj_light_1), 0.3);
	Global::pointLightList.push_back(std::move(pointLight1));

	auto pointLight2 = make_unique<PointLight>(glm::vec3(0.3, 0.3, 0.85), 1.1, glm::vec4(0.7, 1, 1, 1), std::move(obj_light_2), 1.5);
	Global::pointLightList.push_back(std::move(pointLight2));

	auto dirLight1 = make_unique<DirectionLight>(glm::vec3(1, 1, 1), 1, glm::vec4(1, 1, 1, 1), std::move(obj_light_3));
	dirLight1->setTransform(glm::vec3(1, 1, 1), glm::vec3(0,70,0));
	Global::dirLightList.push_back(std::move(dirLight1));
	//----------------------------------------------------------------------------

	//precompute skybox
	Material skyPrecompute;
	creataMaterial<SkyBoxPreCompute>(skyPrecompute);
	auto srcTxt = std::make_shared<Texture>("Model/textures/Road.hdr", TextureType::HDR, false);
	skyPrecompute.shader->block.albedo = srcTxt;
	//auto preSkyObj = std::make_unique<Object>("Model/cube_2.obj", skyPrecompute);
	auto preSkyObj = std::make_unique<Object>(2, skyPrecompute);
	preSkyObj->SetScale(0.1, 0.1, 0.1);
	preSkyObj->SetRotate(0, 10, 0);
	preSkyObj->SetTranslate(0, 0, 0);

	//drawSkybox:
	Material skyboxMat(ShadingType::Skybox);
	creataMaterial<SkyboxShader>(skyboxMat);
	auto skyboxTxt = std::make_shared<Texture3D>("Model/textures/valley_skybox.hdr");
	skyboxMat.shader->block.skybox = skyboxTxt;
	auto skyboxObj = std::make_unique<Object>(2, skyboxMat);
	skyboxObj->SetScale(10, 10, 10);
	skyboxObj->SetRotate(0, 0, 0);
	skyboxObj->SetTranslate(0, 0, 0);


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

	glfwSetScrollCallback(window, scroll_callback);

	glfwSetMouseButtonCallback(window, mouse_btn_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

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
		
		skyboxObj->RenderObject();

		for (size_t i = 0; i < Global::pointLightList.size(); i++)
		{
			Global::pointLightList[i]->obj->RenderObject();
		}
		for (int i = 0; i < Global::dirLightList.size(); i++)
		{
			Global::dirLightList[i]->obj->RenderObject();
		}
		preSkyObj->RenderObject();
		
		//obj.RenderObject();
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




