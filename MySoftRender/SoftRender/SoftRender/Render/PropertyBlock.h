#pragma once
#include"Transform.hpp"
#include"Texture.h"
#include<memory>
namespace softRD
{
	class PropertyBlock
	{
	public:
		PropertyBlock() {}

		~PropertyBlock() {}

		void SetTransform(Transform* _transform)
		{
			transform = _transform;
		}

		std::unique_ptr<Texture> albedo;
		//后面还有pbr的一坨贴图 metallic,roughness,normal,ao ..
		Transform* transform;
	private:

	};

}
