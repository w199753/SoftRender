#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#include"../Math/fMath.h"
softRD::Texture::Texture()
{
}

softRD::Texture::Texture(const std::string& path, TextureType _txtType) {

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
		p = data;
		return internal_Sampler2D(uv, res, p)* Div255;
	}
	else if (txtType == TextureType::HDR)
	{
		fp = fData;
		return internal_Sampler2D(uv, res, fp)* Div255;
	}
	p = nullptr;
	fp = nullptr;
}


