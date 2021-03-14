#include "fMath.h"

float fMath::Distance(const glm::vec3 & src, const glm::vec3 & dest)
{
	return (src - dest).length();
	//return sqrtf((dest.x - src.x)*(dest.x - src.x) + (dest.y - src.y)*(dest.y - src.y) + (dest.z - src.z)*(dest.z - src.z));
}

float fMath::Distance(const glm::vec2 & src, const glm::vec2 & dest)
{
	return sqrtf((dest.x - src.x)*(dest.x - src.x) + (dest.y - src.y)*(dest.y - src.y));
}

glm::mat4 fMath::GetScaleMatrix(const float p)
{
	glm::mat4 res(p);
	res[3][3] = 1;
	res[2][2] = p;
	res[1][1] = p;
	res[0][0] = p;
	return res;
}

glm::mat4 fMath::GetScaleMatrix(const glm::vec3 & v)
{
	glm::mat4 res(1);
	res[0][0] = v.x;
	res[1][1] = v.y;
	res[2][3] = v.z;
	return res;
}

glm::mat4 fMath::GetTranslateMatrix(const glm::vec3 & v)
{
	glm::mat4 res(1);
	res[0][3] = v.x;
	res[1][3] = v.y;
	res[2][3] = v.z;
	return glm::transpose(res);
}

glm::mat4 fMath::GetXRotateMatrix(const float p)
{
	glm::mat4 res(1);
	float DEG2RAD = 0.0174532924f;
	float radian = p*DEG2RAD;
	res[1][1] = cos(radian);
	res[1][2] = -sin(radian);
	res[2][1] = sin(radian);
	res[2][2] = cos(radian);
	return res;
}

glm::mat4 fMath::GetYRotateMatrix(const float p)
{
	glm::mat4 res(1);
	float DEG2RAD = 0.0174532924f;
	float radian = p*DEG2RAD;
	res[0][0] = cos(radian);
	res[0][2] = sin(radian);
	res[2][0] = -sin(radian);
	res[2][2] = cos(radian);
	return res;
}

glm::mat4 fMath::GetZRotateMatrix(const float p)
{
	glm::mat4 res(1);
	float DEG2RAD = 0.0174532924f;
	float radian = p*DEG2RAD;
	res[0][0] = cos(radian);
	res[0][1] = -sin(radian);
	res[1][0] = sin(radian);
	res[1][1] = cos(radian);
	return res;
}

//规定旋转顺序：z->y->x
glm::mat4 fMath::GetRotateMatrix(const glm::vec3 & v)
{
	glm::mat4 res(1);
	res = GetXRotateMatrix(v.x)*GetYRotateMatrix(v.y)*GetZRotateMatrix(v.z);
	return glm::transpose(res);
}

glm::mat4 fMath::GetViewMatrix(const glm::vec3 & trans, const glm::vec3 & rotate)
{
	glm::mat4 res(1);
	res = glm::inverse(GetTranslateMatrix(trans)* (GetRotateMatrix(rotate)));
	return res;
}

glm::mat4 fMath::GetViewMatrix(const glm::vec3 & pos, const glm::vec3 & front, const glm::vec3 & up, const glm::vec3 & right)
{
	glm::mat4 res(1);
	res[0][0] = right.x;
	res[1][0] = right.y;
	res[2][0] = right.z;
	res[3][0] = -glm::dot(right, pos); //-glm::dot(right, pos);
	res[0][1] = up.x;
	res[1][1] = up.y;
	res[2][1] = up.z;
	res[3][1] = -glm::dot(up, pos);
	res[0][2] = -front.x;
	res[1][2] = -front.y;
	res[2][2] = -front.z;
	res[3][2] = glm::dot(front, pos);
	return (res);
}

glm::mat4 fMath::GetProjectMatrix(const float _near, const float _far, const float _fov, const float _aspect)
{
	/*glm::mat4 res(1);
	float fov = (_fov / 2)*DEG2RAD;
	return res;*/

	glm::mat4 result(0);
	const float tanHalfFov = tan(_fov*0.5f);
	result[0][0] = 1.0f / (_aspect*tanHalfFov);
	result[1][1] = 1.0f / (tanHalfFov);
	result[2][2] = -(_far + _near) / (_far - _near);
	result[2][3] = -1.0f;
	result[3][2] = (-2.0f*_near*_far) / (_far - _near);
	return (result);
}

glm::mat4 fMath::GetOrthogonProjectMatrix(const float width, const float height, const float near, const float far)
{
	glm::mat4 res(1);
	res[0][0] = 2.f / width;
	res[1][1] = 2.f / height;
	res[2][2] = -2.f / (far - near);
	res[3][2] = -(far + near) / (far - near);
	return res;
}

glm::mat4 fMath::GetViewPortMatrix(const int ox, const int oy, const int width, const int height)
{
	glm::mat4 result(1);
	result[0][0] = static_cast<float>(width) / 2.0f;
	result[1][1] = static_cast<float>(height) / 2.0f;
	result[3][0] = static_cast<float>(ox) + (width / 2.0f);
	result[3][1] = static_cast<float>(oy) + (height / 2.0f);
	return (result);
}

//view:eyePos
bool fMath::FaceCulling(glm::vec4& a, glm::vec4& b, glm::vec4& c, const glm::vec3 view, bool isFront)
{
	//auto normal = glm::vec3::cross(b - a, c - a).normalize();
	glm::vec4 tmp1 = b - a;
	glm::vec4 tmp2 = c - a;
	glm::vec3 e1(tmp1.x, tmp1.y, tmp1.z);
	glm::vec3 e2(tmp2.x, tmp2.y, tmp2.z);

	glm::vec3 view1 = glm::vec3(a) - view;
	glm::vec3 view2 = glm::vec3(b) - view;
	glm::vec3 view3 = glm::vec3(c) - view;
	//glm::vec3 viewDir = view1+view2+view3;

	glm::vec3 viewDir(view.x - a.x, view.y - a.y, view.z - a.z);
	glm::vec3 normal = glm::normalize(glm::cross(e1, e2));
	if (isFront)
	{
		return  glm::dot(normal, view) < 0;
	}
	else
	{
		return glm::dot(normal, view) > 0;
	}
}

glm::vec3 fMath::GetFaceNormal(const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec3 & v3)
{
	return glm::normalize(glm::vec3((v1 + v2 + v3)));
}

glm::vec4& fMath::Linear2Srgb(glm::vec4& col)
{
	col.x = powf(col.x, 2.2f);
	col.z = powf(col.z, 2.2f);
	col.y = powf(col.y, 2.2f);
	col.w = powf(col.w, 2.2f);
	return col;
}

glm::vec4& fMath::Srgb2Linear(glm::vec4& col)
{
	return glm::vec4();
}

//把法线贴图的rgb值变换到 -1,1的法线域
glm::vec4& fMath::UnPackTexture(glm::vec4& t_col)
{
	t_col =  t_col*2.f -1.f;
	return t_col;
}

//把法线贴图的-1,1法线域变换到rgb的颜色值   (写入贴图时才会用到，一般基本用不到
glm::vec4& fMath::PackTexture(glm::vec4& t_col)
{
	t_col = t_col * 0.5f + 0.5f;
	return t_col;
}
