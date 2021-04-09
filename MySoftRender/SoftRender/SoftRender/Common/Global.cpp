#include"Global.h"
#include "../Render/Camera.h"
#include "../Render/Rasterization.h"
#include"../Render/FrameBuffer.h"
#include"../Light/Light.h"
using namespace softRD;

//constexpr float oneDiv255 = 1 / 255.0f;
//constexpr float PI = 3.1415926;
//constexpr float DEG2RAD = 0.0174532924f;
//constexpr float RAD2DEG = 57.29578;

std::unique_ptr<Camera> Global::mainCamera = nullptr;
std::unique_ptr<Rasterization> Global::raster = nullptr;
std::unique_ptr<FrameBuffer> Global::frameBuffer = nullptr;
std::vector<std::unique_ptr<PointLight>> Global::pointLightList;
std::vector<std::unique_ptr<DirectionLight>> Global::dirLightList;
float Global::triangleCount = 0;
float Global::verticesCount = 0;
//void Global::SetMainCamera(std::unique_ptr<Camera>_main)
//{
//	mainCamera = std::move(_main);
//}
