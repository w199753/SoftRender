#pragma once
#ifndef __FMATH__H__
#define __FMATH__H__

#include<immintrin.h>
#include "math_def.h"
#include<glm/vec4.hpp>
#include<glm/vec3.hpp>
#include<glm/vec2.hpp>
#include<glm/mat4x4.hpp>

class fMath
{
public:

	template<typename T>
	static T Lerp(const T& a, const T& b, const float t);
	//template<>
	//static int Lerp(const int a, const int b, const float t);
	//template<>
	//static V2f Lerp(const V2f& a, const V2f& b, const float t);


	template<typename T>
	static T Clamp(const T& value, const T& a, const T& b);

	template<typename T>
	static T Saturate(const T& value);

	template<>
	static glm::vec4 Saturate<glm::vec4>(const glm::vec4& value);
	
	/*
	a----b
	|		|
	c----d
	*/
	template<typename T>
	static T DoubleLerp(const T& a, const T& b, const T& c, const T& d, const glm::vec2& sourceP);


	//return a,b,c  a=(1-b-c)
	template<class T>
	static glm::vec3  barycentric(const T& a, const T& b, const T& c, const T& p);
	template<>
	static glm::vec3  barycentric(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p);

	template<typename T>
	static bool CheckInTriangle(const T& a, const T& b, const T& c, const T& p);
	template<>
	static bool CheckInTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p);

	static float Distance(const glm::vec3& src, const glm::vec3& dest);
	static float Distance(const glm::vec2& src, const glm::vec2& dest);
	//template <typename T>
	////仅交换stl中元素快吧可能
	//static inline void Swap(T & a, T& b)
	//{
	//	T tmp = move(a);
	//	a = move(b);
	//	b = move(tmp);
	//}
	//modelMatirx
	static glm::mat4 GetScaleMatrix(const float p);
	static glm::mat4 GetScaleMatrix(const glm::vec3& v);
	static glm::mat4 GetTranslateMatrix(const glm::vec3& v);
	static glm::mat4 GetXRotateMatrix(const float p);
	static glm::mat4 GetYRotateMatrix(const float p);
	static glm::mat4 GetZRotateMatrix(const float p);
	static glm::mat4 GetRotateMatrix(const glm::vec3& v);
	//viewMatrix  有两种，一种使用逆变换，一种将位置变换到相机规定的原点位置
	static glm::mat4 GetViewMatrix(const glm::vec3& trans, const glm::vec3& rotate);
	static glm::mat4 GetViewMatrix(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, const glm::vec3& right);
	//projectMatrix
	//透视投影矩阵
	static glm::mat4 GetProjectMatrix(const float near, const float far, const float fov, const float aspect);
	//正交投影矩阵
	static glm::mat4 GetOrthogonProjectMatrix(const float width,const float height,const float near,const float far);
	//static glm::mat4 GetProjectMatrix();
	//viewportMatrix
	static glm::mat4 GetViewPortMatrix(const int ox, const int oy, const int width, const int height);

	static bool FaceCulling(glm::vec4& a, glm::vec4& b, glm::vec4& c, const glm::vec3 view, bool isFront);

	static glm::vec3 GetFaceNormal(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
	static glm::vec4& Linear2Srgb(glm::vec4& col);
	static glm::vec4& Srgb2Linear(glm::vec4& col);
	static glm::vec4& UnPackTexture(glm::vec4& t_col);
	static glm::vec4& PackTexture(glm::vec4& t_col);
};


template<typename T>
T fMath::Lerp(const T& a, const T& b, const float t)
{
	return a * (1 - t) + b * t;
}
//特化来处理mipmap插值
//template<>
//int fMath::Lerp(const int a, const int b, const float t)
//{
//	//cout << "wuhu" << endl;
//	return 1000;
//}

//template<>
//V2f fMath::Lerp(const V2f& a, const V2f& b, const float t)
//{
//	V2f tmp;
//	tmp.color = Lerp<glm::vec4>(a.color, b.color, t);
//	tmp.normal = Lerp<glm::vec3>(a.normal, b.normal, t);
//	tmp.windowPos = Lerp<glm::vec4>(a.windowPos, b.windowPos, t);
//	tmp.worldPos = Lerp<glm::vec4>(a.worldPos, b.worldPos, t);
//	tmp.texcoord = Lerp<glm::vec2>(a.texcoord, b.texcoord, t);
//	return tmp;
//}


template<typename T>
T fMath::Clamp(const T& value, const T& a, const T& b)
{
	if (value < a)return a;
	else if (value > b)return b;
	else return value;
}

template<typename T>
T fMath::Saturate(const T& value)
{
	return Clamp<T>(value, static_cast<T>(0), static_cast<T>(1));
}

template<>
glm::vec4 fMath::Saturate<glm::vec4>(const glm::vec4& v)
{
	return glm::vec4(Saturate(v.x), Saturate(v.y), Saturate(v.z), Saturate(v.w));
}


template<typename T>
T fMath::DoubleLerp(const T& a, const T& b, const T& c, const T& d, const glm::vec2& sourceP)
{
	//float xParam = abs(sourceP.x - c.x);
	//float yParam = abs(sourceP.y - c.y);
	float xParam = sourceP.x;
	float yParam = sourceP.y;

	T xTop = Lerp(a, b, xParam);
	T xBottom = Lerp(c, d, xParam);
	T yRight = Lerp(xBottom, xTop, yParam);
	return yRight;
}


template<class T>
glm::vec3  fMath::barycentric(const T& a, const T& b, const T& c, const T& p)
{
	float rA, rB, rC;
	rA = (-(p.x - b.x)*(c.y - b.y) + (p.y - b.y)*(c.x - b.x)) / (-(a.x - b.x)*(c.y - b.y) + (a.y - b.y)*(c.x - b.x));
	rB = (-(p.x - c.x)*(a.y - c.y) + (p.y - c.y)*(a.x - c.x)) / (-(b.x - c.x)*(a.y - c.y) + (b.y - c.y)*(a.x - c.x));
	rC = 1.0f - rA - rB;
	return glm::vec3(rA, rB, rC);
}
//
template<>
glm::vec3  fMath::barycentric(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p)
{
	return glm::vec3();
}

template<typename T>
inline bool fMath::CheckInTriangle(const T & a, const T & b, const T & c, const T & p)
{
	glm::vec3 res = barycentric<T>(a, b, c, p);
	if (res.x > 0 && res.y > 0 && res.z > 0)
		return true;
	return false;
}
template<>
inline bool fMath::CheckInTriangle(const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c, const glm::vec3 & p)
{
	glm::vec3 res = barycentric(a, b, c, p);
	if (res.x > 0 && res.y > 0 && res.z > 0)
		return true;
	return false;
}


#endif