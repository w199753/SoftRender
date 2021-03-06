#include "FrameBuffer.h"
#include "../Common/Global.h"
#include"../Math/fMath.h"
#include<algorithm>

using namespace softRD;

void FrameBuffer::ClearBuffer(BuffersType type)
{
	if ((type&BuffersType::COLOR) == BuffersType::COLOR)
	{
		auto colData = colorBuffer.data();
		for (size_t i = 0; i < colorBuffer.size(); i+=4)
		{
			*(colData + i) = 0;
			*(colData + i + 1) = 0;
			*(colData + i + 2) = 0;
			*(colData + i + 3) = 255;
		}
		colData = nullptr;
	}
	if ((type & BuffersType::DEPTH) == BuffersType::DEPTH)
	{
		std::fill(depthBuffer.begin(), depthBuffer.end(), 1);
	}
}

void FrameBuffer::WriteColor(int x, int y, const glm::vec4& col)
{
	if (x<0||x>=width||y<0||y>=height)
	{
		return;
	}
	int xy = 4*(y * width + x);
	auto colItr = colorBuffer.begin() + xy;
	*(colItr) = fMath::Clamp((int)(col.x * 255), 0, 255);
	*(colItr + 1) = fMath::Clamp((int)(col.y * 255), 0, 255);
	*(colItr + 2) = fMath::Clamp((int)(col.z * 255), 0, 255);
	*(colItr + 3) = fMath::Clamp((int)(col.w * 255), 0, 255);
}

void softRD::FrameBuffer::WriteDepth(int x, int y, const float v)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return;
	}
	float* p = depthBuffer.data();
	int xy = (y * width + x);
	*(p + (y * width + x)) = v;
}

float softRD::FrameBuffer::GetDepth(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return std::numeric_limits<float>::infinity();
	}
	float* p = depthBuffer.data();
	int xy = (y * width + x);
	return *(p + (y * width + x));
}

glm::vec4 FrameBuffer::GetColor(int x, int y)
{
	int xy = 4 * (y * width + x);
	float oneDiv255 = 1 / 255.f;
	return glm::vec4(colorBuffer[xy],colorBuffer[xy+1],colorBuffer[xy+2],colorBuffer[xy+3])*oneDiv255;
}
