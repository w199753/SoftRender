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
		//后面还有pbr的一坨贴图 metallic,roughness,normal,ao ..
		std::shared_ptr<Texture3D> skybox;
		std::unique_ptr<Transform> transform;
	private:

	};

}
