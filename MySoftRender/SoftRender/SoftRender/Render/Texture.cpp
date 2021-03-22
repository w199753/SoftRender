#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#include"../Math/fMath.h"
softRD::Texture::Texture()
{
}

softRD::Texture::Texture(const std::string& path, TextureType _txtType, bool _isGenMipmap) {
	isGenMipmap = _isGenMipmap;
	txtType = _txtType;
	if (txtType == TextureType::LDR)
	{
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(path.c_str(), &width, &height, &channel, 0);
	}
	else if (txtType == TextureType::HDR)
	{
		stbi_set_flip_vertically_on_load(true);
		fData = stbi_loadf(path.c_str(), &width, &height, &channel, 0);
	}

	if (isGenMipmap)
	{
		int level = 0;
		int maxLength = std::max(width, height);
		auto srcData = data;

		auto baseMap = std::make_unique<Texture>();
		baseMap->channel = channel;
		baseMap->width = width;
		baseMap->height = height;
		baseMap->wrapType = wrapType;
		baseMap->filterType = filterType;
		baseMap->gammaCorrect = gammaCorrect;
		baseMap->isGenMipmap = false;
		baseMap->data = new unsigned char[width * height * channel];
		std::copy(data, data + width * height * channel, baseMap->data);
		mipmapData.push_back(std::move(baseMap));
		level++;

		for (size_t i = maxLength >> 1; i > 0; i >>= 1)
		{

			auto mip = std::make_unique<Texture>();
			mip->channel = channel;
			mip->width = i;
			mip->height = i;
			mip->wrapType = wrapType;
			mip->filterType = filterType;
			mip->gammaCorrect = gammaCorrect;
			mip->isGenMipmap = false;
			mip->data = new unsigned char[i * i * channel];

			unsigned char* tarData = new unsigned char[i * i * channel];
			int xIdx = 0, yIdx = 0;
			int lastWidth = mipmapData[level - 1]->width;
			unsigned char* lastData = mipmapData[level - 1]->data;
			for (size_t y = 0; y < 2*i-1; y += 2)
			{
				xIdx = 0;
				for (size_t x = 0; x < 2*i-1; x += 2)
				{
					int xy1 = (y * lastWidth + x) * channel;
					int xy2 = (y * lastWidth + ((x + 1) > i ? x : (x + 1))) * channel;
					int xy3 = (((y + 1) > i ? y : (y + 1)) * lastWidth + x) * channel;
					int xy4 = (((y + 1) > i ? y : (y + 1)) * lastWidth + ((x + 1) > i ? x : (x + 1))) * channel;

					int tarIdx = (yIdx * mip->width + xIdx) * channel;
					for (size_t c = 0; c < channel; c++)
					{
						auto advCol = (*(lastData + xy1 + c) + *(lastData + xy2 + c) + *(lastData + xy3 + c) + *(lastData + xy4 + c)) / 4;
						*(tarData + tarIdx + c) = advCol;
						//*(tarData)
					}
					xIdx++;

				}
				yIdx++;
				//std::cout << xIdx << " " << yIdx << std::endl;
			}
			
			std::copy(tarData, tarData + i * i * channel, mip->data);
			level++;
			mipmapData.push_back(std::move(mip));
		}

	}

}

softRD::Texture::Texture(const Texture& t)
{
	this->width = t.width;
	this->height = t.height;
	this->channel = t.channel;
	this->filterType = t.filterType;
	this->wrapType = t.wrapType;
	this->gammaCorrect = t.gammaCorrect;
	this->txtType = t.txtType;
	const int len = width * height * channel;
	if (this->data != nullptr)
	{
		delete this->data;
	}
	if (this->fData != nullptr)
	{
		delete this->fData;
	}

	//std::copy(t.data, t.data + len, this->data);
	if (t.data)
	{
		this->data = new  unsigned char[len];
		memcpy_s(this->data, len, t.data, len);
	}
	if (t.fData)
	{
		this->fData = new float[len];
		std::copy(t.fData, t.fData + len, this->fData);
	}
}


softRD::Texture& softRD::Texture::operator = (const Texture& t)
{
	if (this == &t)
	{
		return *this;
	}
	this->width = t.width;
	this->height = t.height;
	this->channel = t.channel;
	this->filterType = t.filterType;
	this->txtType = t.txtType;
	this->wrapType = t.wrapType;
	this->gammaCorrect = t.gammaCorrect;
	const int len = width * height * channel;
	if (this->data != nullptr)
	{
		delete this->data;
	}
	if (this->fData != nullptr)
	{
		delete this->fData;
	}
	if (t.data)
	{
		this->data = new  unsigned char[len];
		std::copy(t.data, t.data + len, this->data);
	}
	if (t.fData)
	{
		this->fData = new float[len];
		std::copy(t.fData, t.fData + len, this->fData);
	}
	return *this;
}

glm::vec4 softRD::Texture::Sampler2D(const glm::vec2& uv)
{
	//std::cout  << uv.x << " ??????" << uv.y << std::endl;
	glm::vec4 res = glm::vec4(0);
	unsigned char* p;
	float* fp;
	float Div255 = 1.f / 255.f;
	if (txtType == TextureType::LDR)
	{
		//p = data;
		p = mipmapData[0]->data;
		return internal_Sampler2D(uv, res, p) * Div255;
	}
	else if (txtType == TextureType::HDR)
	{
		fp = fData;
		return internal_Sampler2D(uv, res, fp) * Div255;
	}
	p = nullptr;
	fp = nullptr;
}


