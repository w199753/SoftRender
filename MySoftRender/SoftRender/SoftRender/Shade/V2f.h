#pragma once
#include<glm/vector_relational.hpp>
namespace softRD
{
	class V2f
	{
	public:
		V2f() {}
		~V2f() {}

		V2f(const V2f& v2f)
		{
			worldPos = v2f.worldPos;
			windowPos = v2f.windowPos;
			color = v2f.color;
			normal = v2f.normal;
			texcoord = v2f.texcoord;
			Z = v2f.Z;
		}

		V2f& operator = (const V2f& v2f)
		{
			if (this == &v2f)
			{
				return *this;
			}
			worldPos = v2f.worldPos;
			windowPos = v2f.windowPos;
			color = v2f.color;
			normal = v2f.normal;
			texcoord = v2f.texcoord;
			Z = v2f.Z;
			return *this;
		}

		V2f operator*(const float p)const
		{
			V2f o;
			o.worldPos = worldPos * p;
			o.windowPos = windowPos * p;
			o.normal = p * normal;
			o.color = p * color;
			o.texcoord = p * texcoord;
			o.Z = p * Z;
			return o;
		}

		V2f operator+(const V2f& v2f) const
		{
			V2f res;
			res.worldPos = worldPos + v2f.worldPos;
			res.windowPos = windowPos + v2f.windowPos;
			res.color = color + v2f.color;
			res.normal = normal + v2f.normal;
			res.texcoord = texcoord + v2f.texcoord;
			res.Z = Z + v2f.Z;
			return res;
		}

		glm::vec4 worldPos;
		glm::vec4 windowPos;
		glm::vec4 color;
		glm::vec3 normal;
		glm::vec2 texcoord;
		float Z;
		//glm::vec3 faceNormal;
		//glm::vec4 faceColor;
	};
}
