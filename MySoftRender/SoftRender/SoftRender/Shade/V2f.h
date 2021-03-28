#pragma once
#include<glm/vector_relational.hpp>
namespace softRD
{
	class V2f
	{
	public:
		glm::vec4 worldPos;
		glm::vec4 windowPos;
		glm::vec4 color;
		glm::vec3 normal;
		glm::vec2 texcoord;
		float		Z;

		V2f() :worldPos(glm::vec4(0)), windowPos(glm::vec4(0)), color(glm::vec4(0)), normal(glm::vec3(0)), texcoord(glm::vec2(0)), Z(0) {
		}
		V2f(const glm::vec4& _worldPos,
			const glm::vec4& _windowPos,
			const glm::vec4& _color,
			const glm::vec3& _normal,
			const glm::vec2& _texcoord,
			float _z = 1.0f) :
			worldPos(_worldPos), windowPos(_windowPos), color(_color), normal(_normal), texcoord(_texcoord), Z(_z) {}

		~V2f() {}

		//V2f(const V2f& v2f)
		//{
		//	worldPos = v2f.worldPos;
		//	windowPos = v2f.windowPos;
		//	color = v2f.color;
		//	normal = v2f.normal;
		//	texcoord = v2f.texcoord;
		//	Z = v2f.Z;
		//}
		//
		//V2f& operator = (V2f&& v2f)noexcept
		//{
		//	worldPos = v2f.worldPos;
		//	windowPos = v2f.windowPos;
		//	color = v2f.color;
		//	normal = v2f.normal;
		//	texcoord = v2f.texcoord;
		//	Z = v2f.Z;
		//	return *this;
		//}

		//V2f& operator = (const V2f& v2f)
		//{
		//	//if (this == &v2f)
		//	//{
		//	//	return *this;
		//	//}
		//	worldPos = v2f.worldPos;
		//	//windowPos = v2f.windowPos;
		//	//color = v2f.color;
		//	//normal = v2f.normal;
		//	//texcoord = v2f.texcoord;
		//	Z = v2f.Z;
		//	return *this;
		//}

		V2f operator*(const float p)const
		{
			return V2f(worldPos * p, windowPos * p, color * p, normal * p, texcoord * p, Z * p);
		}

		V2f operator+(const V2f& v2f) const
		{
			return V2f(worldPos + v2f.worldPos, windowPos + v2f.windowPos, color + v2f.color, normal + v2f.normal, texcoord + v2f.texcoord, Z + v2f.Z);
		}


		//glm::vec3 faceNormal;
		//glm::vec4 faceColor;
	};
}
