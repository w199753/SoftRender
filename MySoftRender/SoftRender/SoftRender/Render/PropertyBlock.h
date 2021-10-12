#pragma once
#include"Transform.hpp"
#include"Texture.h"
#include<memory>
namespace softRD
{
	class PropertyBlock
	{
	public:
		PropertyBlock() { transform = std::make_unique<Transform>(); }

		~PropertyBlock() {}

		void SetTransform(std::unique_ptr<Transform> _transform)
		{
			transform = std::move(_transform);
		}

		std::shared_ptr<Texture> albedo;
		std::shared_ptr<Texture> normal;
		std::shared_ptr<Texture> metallic;
		std::shared_ptr<Texture> roughness;
		std::shared_ptr<Texture> ao;
		std::shared_ptr<Texture> emission;
		//���滹��pbr��һ����ͼ metallic,roughness,normal,ao ..
		std::shared_ptr<Texture3D> skybox;
		std::shared_ptr<Texture3D> irradiance;
		std::shared_ptr<Texture3D> radiance;
		std::shared_ptr<Texture> brdf;

		std::unique_ptr<Transform> transform;
	private:

	};

}
