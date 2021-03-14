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
		//���滹��pbr��һ����ͼ metallic,roughness,normal,ao ..
		Transform* transform;
	private:

	};

}
