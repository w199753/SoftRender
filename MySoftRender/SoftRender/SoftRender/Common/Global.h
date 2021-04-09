#pragma once

#include<memory>
#include<vector>
namespace softRD
{

	class Camera;
	class Rasterization;
	class FrameBuffer;
	class PointLight;
	class DirectionLight;
	class Global
	{
	public:
		Global() {}
		~Global() {}

		//void SetMainCamera(std::unique_ptr<Camera>_main);
		static std::unique_ptr<Rasterization> raster;
		static std::unique_ptr<Camera> mainCamera;
		static std::unique_ptr<FrameBuffer> frameBuffer;
		static std::vector<std::unique_ptr<PointLight>> pointLightList;
		static std::vector<std::unique_ptr<DirectionLight>> dirLightList;
		//static std::vector<DirectionLight> dirLightList;
		static float triangleCount;
		static float verticesCount;
	};

}
