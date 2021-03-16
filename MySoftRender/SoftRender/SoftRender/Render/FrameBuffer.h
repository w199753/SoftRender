#pragma once
#include<vector>
#include<glm/vec4.hpp>
namespace softRD
{
	enum class BuffersType
	{
		COLOR = 0x0001,
		DEPTH = 0x0002,
	};
	inline BuffersType operator |(BuffersType a, BuffersType b)
	{
		return BuffersType((int)a | (int)b);
	}
	inline BuffersType operator &(BuffersType a, BuffersType b)
	{
		return BuffersType((int)a & (int)b);
	}


	class FrameBuffer
	{
	public:
		FrameBuffer(int width = 800, int height = 600)
		{
			colorBuffer.resize(width * height * 4, 0);
			depthBuffer.resize(width * height, 1.f);
			this->width = width;
			this->height = height;
		}
		~FrameBuffer()
		{

		}

		void ClearBuffer(BuffersType type);
		void WriteColor(int x, int y, const glm::vec4& col);
		void WriteDepth(int x, int y, const float v);
		float GetDepth(int x, int y);
		glm::vec4 GetColor(int x, int y);


		int width, height;
		std::vector<unsigned char> colorBuffer;   //unsigned用来表示byte char（-128，127）   unsigned char（0-255）
		std::vector<float> depthBuffer;
	private:


	};
}
