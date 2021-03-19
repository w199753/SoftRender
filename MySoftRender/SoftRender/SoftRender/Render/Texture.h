#pragma once
#include<iostream>
#include<vector>
#include<glm/vector_relational.hpp>
#include<glm/glm.hpp>
#include<glm/common.hpp>
#include<glm/ext.hpp>
namespace softRD
{
	enum class FilterType
	{
		Point,
		Bilinear,
	};
	enum class TextureType
	{
		LDR,
		HDR,
	};
	enum class WrapType
	{
		Clamp,
		Repeat,
	};
	class Texture
	{
	public:
		Texture();
		Texture(const std::string& path, TextureType _txtType,bool isGenMipmap = false);

		Texture::Texture(const Texture& t);

		Texture& Texture::operator = (const Texture& t);

		glm::vec4 Sampler2D(const glm::vec2& uv);

		template<typename T>
		glm::vec4& internal_Sampler2D(const glm::vec2& uv,glm::vec4& res,const T* p)
		{
			int xy = 0;
			if (filterType == FilterType::Point)
			{
				int x = 0;
				int y = 0;
				if (wrapType == WrapType::Clamp)
				{
					x = (int)(uv.x * width - 0.5f) ;
					y = (int)(uv.y * height - 0.5f) ;
					x = fMath::Clamp(x, 0, width);
					y = fMath::Clamp(y, 0, height);
				}
				else if (wrapType == WrapType::Repeat)
				{
					x = (int)(uv.x * width - 0.5f) % width;
					y = (int)(uv.y * height - 0.5f) % height;
					x = x < 0 ? width + x : x;
					y = y < 0 ? height + y : y;
				}
				xy = (y * width + x) * channel;
			}
			else if (filterType == FilterType::Bilinear)
			{

			}

			if (channel>=1)
			{
				res.x = *(p + xy);
			}
			if (channel >= 2)
			{
				res.y = *(p + xy + 1);
			}
			if (channel >= 3)
			{
				res.z = *(p + xy + 2);
			}
			if (channel >= 4)
			{
				res.w = *(p + xy + 3);
			}

			//std::cout << res.x << " " << res.y << " " << res.z << " " << channel<<" "<<uv.x<<" "<<uv.y << std::endl;
			return res;
		}

		~Texture() {}

		int width, height;
		int channel;
		bool isGenMipmap = false;
		bool gammaCorrect = false;
		unsigned char* data;
		float* fData;
		FilterType filterType = FilterType::Point;
		TextureType txtType = TextureType::LDR;
		WrapType wrapType = WrapType::Clamp;

	private:
		std::vector<std::unique_ptr<Texture>> mipmapData;
		void GenMipmap()
		{

		}
	};

}