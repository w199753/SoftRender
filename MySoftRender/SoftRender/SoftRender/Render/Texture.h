#pragma once
#include<iostream>
#include<vector>
#include<glm/vector_relational.hpp>
#include<glm/glm.hpp>
#include<glm/common.hpp>
#include<glm/ext.hpp>
using namespace std;
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
		Texture(const std::string& path, TextureType _txtType, bool isGenMipmap = false);

		Texture::Texture(const Texture& t);

		Texture::Texture(int width, int height, int channel,TextureType txtType);

		Texture& Texture::operator = (const Texture& t);

		glm::vec4 Sampler2D(const glm::vec2& uv);

		template<typename T>
		glm::vec4 internal_Sampler2D(const glm::vec2& uv, glm::vec4& res, const T p)
		{
			int width, height;
			if (isGenMipmap)
			{
				width = mipmapData[0]->width;
				height = mipmapData[0]->height;
			}
			else
			{
				width = this->width;
				height = this->height;
			}

			if (filterType == FilterType::Point)
			{
				int xy = 0;
				int x = 0;
				int y = 0;
				if (wrapType == WrapType::Clamp)
				{
					x = (int)(uv.x * width - 0.5f);
					y = (int)(uv.y * height - 0.5f);
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
				if (channel >= 1) { res.x = *(p + xy); }
				if (channel >= 2) { res.y = *(p + xy + 1); }
				if (channel >= 3) { res.z = *(p + xy + 2); }
				if (channel >= 4) { res.w = *(p + xy + 3); }
			}
			else if (filterType == FilterType::Bilinear)
			{
				float srcx = uv.x * width - 0.5f;
				float srcy = uv.y * height - 0.5f;


				if (wrapType == WrapType::Clamp)    //repeat模式的双线性插值懒得写了
				{
					
					
				}
				else if (wrapType == WrapType::Repeat)
				{
					if (srcx > width)srcx = srcx -(float)width;
					if (srcy > height)srcy = srcy -(float)height;
				}
				int umin = floor(srcx);
				int vmin = floor(srcy);
				int umax = ceil(srcx);
				int vmax = ceil(srcy);
				/*
				x1-----x2
				|	|	|
				|---x---|
				|	|	|
				x3-----x4
				*/
				//srcx = fMath::Clamp(srcx, 0.f, (width - 1) * 1.0f);
				//srcy = fMath::Clamp(srcy, 0.f, (height - 1) * 1.0f);
				int clampedUmin = fMath::Clamp(umin, 0, (width - 1));
				int clampedVmin = fMath::Clamp(vmin, 0, (height - 1));
				int clampedUmax = fMath::Clamp(umax, 0, (width - 1));
				int clampedVmax = fMath::Clamp(vmax, 0, (height - 1));
				int x1 = clampedUmin;
				int y1 = clampedVmin;
				int xy1 = (y1 * width + x1) * channel;
				int x2 = clampedUmax;
				int y2 = clampedVmin;
				int xy2 = (y2 * width + x2) * channel;
				int x3 = clampedUmin;
				int y3 = clampedVmax;
				int xy3 = (y3 * width + x3) * channel;
				int x4 = clampedUmax;
				int y4 = clampedVmax;
				int xy4 = (y4 * width + x4) * channel;
				glm::vec4 col1 = glm::vec4(0);
				glm::vec4 col2 = glm::vec4(0);
				glm::vec4 col3 = glm::vec4(0);
				glm::vec4 col4 = glm::vec4(0);
				if (channel >= 1) {
					col1.x = *(p + xy1);
					col2.x = *(p + xy2);
					col3.x = *(p + xy3);
					col4.x = *(p + xy4);
				}
				if (channel >= 2) {
					col1.y = *(p + xy1 + 1);
					col2.y = *(p + xy2 + 1);
					col3.y = *(p + xy3 + 1);
					col4.y = *(p + xy4 + 1);
				}
				if (channel >= 3) {
					col1.z = *(p + xy1 + 2);
					col2.z = *(p + xy2 + 2);
					col3.z = *(p + xy3 + 2);
					col4.z = *(p + xy4 + 2);
				}
				if (channel >= 4) {
					col1.w = *(p + xy1 + 3);
					col2.w = *(p + xy2 + 3);
					col3.w = *(p + xy3 + 3);
					col4.w = *(p + xy4 + 3);
				}

				float dU = ((x4 - x1) == 0 ? 1.f : (float)(x4 - x1));
				float dV = ((y4 - y1) == 0 ? 1.f : (float)(y4 - y1));
				glm::vec4 top = fMath::Lerp(col1, col2, (srcx - x1) / dU);
				glm::vec4 bottom = fMath::Lerp(col3, col4, (srcx - x1) / dU);
				res = fMath::Lerp(top, bottom, (srcy - y1) / dV);
				//std::cout << res.x<<" "<<res.y<<" "<<res.z << std::endl;
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
		void GenMipmap();
	};

	class Texture3D
	{
	public:

		enum FaceDir {
			FRONT,
			BACK,
			LEFT,
			RIGHT,
			TOP,
			BOTTOM
		};

		Texture3D(const string& path);
		Texture3D(const Texture3D& texture);
		Texture3D& operator = (const Texture3D& texture);

		glm::vec4 SamplerCube(const glm::vec3& v);

		int width;
		int height;
		int channel;
		//unsigned char* data;

		//0:front    1:back    2:left    3:right    4:top    5:bottom
		int faceDir[6];
		vector<Texture*> faces;
	};

}