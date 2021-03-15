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

		std::unique_ptr<Texture> albedo;
		//后面还有pbr的一坨贴图 metallic,roughness,normal,ao ..
		std::unique_ptr<Transform> transform;
	private:

	};

}
