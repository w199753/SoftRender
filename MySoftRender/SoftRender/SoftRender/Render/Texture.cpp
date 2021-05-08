#include "Texture.h"
#include<numeric>
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
		//stbi_set_flip_vertically_on_load(true);
		fData = stbi_loadf(path.c_str(), &width, &height, &channel, 0);
	}

	if (isGenMipmap)
	{	
		GenMipmap();
	}
}

softRD::Texture::Texture(int width, int height, int channel,TextureType txtType)
{
	this->width = width;
	this->height = height;
	this->channel = channel;
	this->txtType = txtType;
	this->filterType = FilterType::Bilinear;
	this->wrapType = WrapType::Clamp;
	if (txtType == TextureType::LDR)
	{
		this->data = new unsigned char[width * height * channel];
	}
	else
	{
		this->fData = new float[width * height * channel];
	}
}

void softRD::Texture::GenMipmap()
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
		for (size_t y = 0; y < 2 * i - 1; y += 2)
		{
			xIdx = 0;
			for (size_t x = 0; x < 2 * i - 1; x += 2)
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
				}
				xIdx++;

			}
			yIdx++;
		}
		std::copy(tarData, tarData + i * i * channel, mip->data);
		level++;
		mipmapData.push_back(std::move(mip));
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
		if (isGenMipmap)
		{
			p = mipmapData[0]->data;
		}
		else
		{
			p = this->data;
		}
		return internal_Sampler2D(uv, res, p) * Div255;
	}
	else if (txtType == TextureType::HDR)
	{
		fp = fData;
		return internal_Sampler2D(uv, res, fp) ;
	}
	p = nullptr;
	fp = nullptr;
}






softRD::Texture3D::Texture3D(const string& path)
{
	iota(faceDir, faceDir + 6, 0); //初始化六个面
	faces.reserve(6);
	vector<glm::vec2> faceTexcoord(6);
	Texture* texture = new Texture(path,TextureType::HDR);

	width = texture->width / 4;
	height = texture->height / 3;
	channel = texture->channel;

	faceTexcoord[LEFT] = glm::vec2(0, height);
	faceTexcoord[BACK] = glm::vec2(width, height);
	faceTexcoord[RIGHT] = glm::vec2(2 * width, height);
	faceTexcoord[FRONT] = glm::vec2(3 * width, height);
	faceTexcoord[TOP] = glm::vec2(width, 2 * height);
	faceTexcoord[BOTTOM] = glm::vec2(width, 0);

	for (int i = 0; i < 6; i++)
	{
		Texture* tmp = new Texture(width, height, channel,texture->txtType);
		if (texture->txtType == TextureType::LDR)
		{
			auto srcData = texture->data;
			auto destData = tmp->data;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int destXY = (y * width + x) * channel;
					int srcX = faceTexcoord[i].x + x;
					int srcY = faceTexcoord[i].y + y;
					int srcXY = (srcY * texture->width + srcX) * channel;
					for (int j = 0; j < channel; j++)
					{
						*(destData + destXY + j) = *(srcData + srcXY + j);
					}
				}
			}
		}
		else
		{
			auto srcData = texture->fData;
			auto destData = tmp->fData;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int destXY = (y * width + x) * channel;
					int srcX = faceTexcoord[i].x + x;
					int srcY = faceTexcoord[i].y + y;
					int srcXY = (srcY * texture->width + srcX) * channel;
					for (int j = 0; j < channel; j++)
					{
						*(destData + destXY + j) = *(srcData + srcXY + j);
					}
				}
			}
		}

		faces.push_back(tmp);
		//delete tmp;
	}

	delete texture;
}

softRD::Texture3D::Texture3D(const Texture3D& t)
{
	this->width = t.width;
	this->height = t.height;
	this->channel = t.channel;
	//cout << "???"<<t.faces[0]->width << endl;
	for (int i = 0; i < 6; i++) {
		//cout << i << endl;
		//cout << t.faces[i]->channel << endl;
		faces.push_back(new Texture(*t.faces[i]));
		//cout << faces[i]->channel << endl;
	}
	//std::copy(t.faces.begin(),t.faces.end(), this->faces.begin());
}

softRD::Texture3D& softRD::Texture3D::operator=(const Texture3D& texture)
{
	// TODO: 在此处插入 return 语句
	return *this;
}

glm::vec4 softRD::Texture3D::SamplerCube(const glm::vec3& v)
{
	float absX = fabs(v.x);
	float absY = fabs(v.y);
	float absZ = fabs(v.z);
	float maxAxis = max(max(absX, absY), absZ);  //获取最大的分量，来获取大致在什么面上采样
	float sc, tc;
	FaceDir faceIndex;
	//标准右手系
	if (maxAxis == absX)//在左右面上决定
	{
		if (v.x > 0)
		{
			faceIndex = RIGHT;
			sc = -v.z;
			tc = -v.y;
		}
		else
		{
			faceIndex = LEFT;
			sc = +v.z;
			tc = -v.y;
		}
	}
	else if (maxAxis == absY)//在上下面上决定
	{
		if (v.y > 0)
		{
			faceIndex = BOTTOM;
			sc = +v.x;
			tc = +v.z;
		}
		else
		{
			faceIndex = TOP;
			sc = +v.x;
			tc = -v.z;
		}
	}
	else //在前后面上决定
	{
		if (v.z > 0)
		{
			faceIndex = BACK;
			sc = +v.x;
			tc = -v.y;
		}
		else
		{
			faceIndex = FRONT;
			sc = -v.x;
			tc = -v.y;
		}
	}

	glm::vec2 uv = glm::vec2((sc / maxAxis + 1) * 0.5f, (tc / maxAxis + 1) * 0.5f);//因为面上图片的中心是（0,0），要转为纹理坐标，所以变换到左下角为0,0
																				 //cout << faces[faceIndex]->channel<< endl;
																				 //if (faces[faceIndex] == NULL)cout << "null" << endl;
	return faces[faceIndex]->Sampler2D(uv);
}


