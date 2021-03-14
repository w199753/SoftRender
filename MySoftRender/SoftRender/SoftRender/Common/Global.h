#pragma once

#include<memory>

namespace softRD
{

	class Camera;
	class Rasterization;
	class FrameBuffer;
	class Global
	{
	public:
		Global() {}
		~Global() {}

		//void SetMainCamera(std::unique_ptr<Camera>_main);
		static std::unique_ptr<Rasterization> raster;
		static std::unique_ptr<Camera> mainCamera;
		static std::unique_ptr<FrameBuffer> frameBuffer;
	};

}
