#pragma once

#ifndef __VEC__H__
#define __VEC__H__

/*
说明：
写了一坨屎，simd也是负优化······ 也不知道自己写这东西干啥
*/

#include<immintrin.h>
#include<iostream>
#include"math_def.h"
using namespace std;

// 平台判断
#if defined(__GNUG__)
# define COMPILER_GCC
#elif defined(_MSC_VER)
# define COMPILER_MSVC
// i don't care about your debug symbol issues...
# pragma warning(disable:4786)
#else
# error "Could not determine compiler"
#endif

#if defined( __SYMBIAN32__ ) 
#   define OS_SYMBIAN
#elif defined( __WIN32__ ) || defined( _WIN32 )
#   define OS_WIN32
#elif defined( __APPLE_CC__)
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define OS_IOS
#   else
#       define OS_IOS
#       define OS_APPLE
#   endif
#elif defined(__ANDROID__)
#	define OS_ANDROID
#else
#	error "Could not determine OS"
#endif

#if defined (OS_ANDROID) || defined( OS_IOS ) || defined( OS_APPLE )
#	define OS_LINUX
#endif


#define _CRT_ALIGN(x) __declspec(align(x))
_CRT_ALIGN(8) struct vec2
{
public:
	vec2() {}

	union
	{
		_CRT_ALIGN(8) float m[2];
		_CRT_ALIGN(8) struct { float x, y; };
	};



	vec2(const float px, const float py)
	{
		x = px;
		y = py;
	}

	friend ostream & operator << (ostream &out, const vec2& v)
	{
		out << v.x << " " << v.y;
		//out << v.v[0] << v.v[1] << v.v[2];
		return out;
	}



	vec2 operator *(const float p)const
	{
		return vec2(x*p, y*p);
	}

	vec2& operator *=(const float p)
	{
		x *= p;
		y *= p;
		return *this;
	}

	vec2 operator +(const vec2& v)const
	{
		return vec2(x + v.x, y + v.y);
	}

	vec2& operator +=(const vec2 v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}


};

ALIGN struct vec3
{
public:
	union
	{
		float v[3];
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { vec2 xy; float z; };
#ifdef USE_SIMD
		__m128 m128;
#endif // USE_SIMD
	};

	vec3() { }

	vec3& operator = (const vec3& v)
	{
		if (this == &v)
		{
			return *this;
		}
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	vec3(const float px, const float py, const float pz)
	{
#ifdef USE_SIMD
		m128 = _mm_set_ps(0.0f, pz, py, px);
#else
		x = px;
		y = py;
		z = pz;
#endif // USE_SIMD
	}

	vec3(const vec2& pxy, const float pz)
	{
#ifdef USE_SIMD
		m128 = _mm_set_ps(0.0f, pz, pxy.y, pxy.x);
#else
		x = pxy.x;
		y = pxy.y;
		z = pz;
#endif // USE_SIMD
	}

	vec3(const float px)
	{
#ifdef USE_SIMD
		m128 = _mm_set_ps1(px);
#else
		x = px;
		y = px;
		z = px;
#endif // USE_SIMD
	}

	inline vec3& operator +=(const vec3& v)
	{
#ifdef USE_SIMD
		m128 = _mm_add_ps(m128, v.m128);
#else
		x += v.x;
		y += v.y;
		z += v.z;
#endif // USE_SIMD
		return *this;
	}

	inline vec3& operator -=(const vec3& v)
	{
#ifdef USE_SIMD
		m128 = _mm_sub_ps(m128, v.m128);
#else
		x -= v.x;
		y -= v.y;
		z -= v.z;
#endif // USE_SIMD
		return *this;
	}

	inline vec3& operator *=(const float p)
	{
#ifdef USE_SIMD
		__m128 div = _mm_set_ps1(p);
		m128 = _mm_mul_ps(m128, div);
#else
		x *= p;
		y *= p;
		z *= p;
#endif // USE_SIMD
		return *this;
	}

	inline vec3& operator /=(const float p)
	{
#ifdef USE_SIMD
		__m128 div = _mm_set_ps1(p);
		m128 = _mm_div_ps(m128, div);
#else
		x /= p;
		y /= p;
		z /= p;
#endif // USE_SIMD
		return *this;
	}

	inline vec3 operator +(const vec3& v)const
	{
#ifdef USE_SIMD
		//cout << "USE_SIMD" << endl;
		vec3 tmp;
		tmp.m128 = _mm_add_ps(m128, v.m128);
#else
		vec3 tmp(x + v.x, y + v.y, z + v.z);
#endif // USE_SIMD
		return tmp;
	}

	inline vec3 operator -(const vec3& v)const
	{
#ifdef USE_SIMD
		vec3 tmp;
		tmp.m128 = _mm_sub_ps(m128, v.m128);
		//m128 = _mm_sub_ps(m128, v.m128);
#else
		vec3 tmp(x - v.x, y - v.y, z - v.z);
#endif // USE_SIMD
		return tmp;
	}

	inline vec3 operator *(const float p)const
	{
#ifdef USE_SIMD
		vec3 tmp;
		tmp.m128 = _mm_mul_ps(m128, _mm_set_ps1(p));
#else
		vec3 tmp(x*p, y*p, z*p);
#endif // USE_SIMD
		return tmp;
	}

	inline vec3 operator /(const float p)
	{
#ifdef USE_SIMD
		__m128 div = _mm_set_ps1(p);
		vec3 tmp;
		tmp.m128 = _mm_div_ps(m128, div);
#else
		vec3 tmp(x / p, y / p, z / p);
#endif // USE_SIMD
		return tmp;
	}

	inline vec3 operator - ()
	{
#ifdef USE_SIMD
		vec3 tmp;
		tmp.m128 = _mm_setzero_ps();
		tmp.m128 = _mm_sub_ps(tmp.m128, m128);
#else
		vec3 tmp(-x, -y, -z);
#endif // USE_SIMD
		return tmp;
	}

	bool operator == (const vec3& p)const
	{
		if (abs(x - p.x) < EQUAL_PRECISION&&abs(y - p.y) < EQUAL_PRECISION&&abs(z - p.z) < EQUAL_PRECISION)
			return true;
		return false;
	}

	bool operator !=(const vec3& p)const
	{
		if (abs(x - p.x) > EQUAL_PRECISION || abs(y - p.y) > EQUAL_PRECISION || abs(z - p.z) > EQUAL_PRECISION)
			return true;
		return false;
	}

	friend ostream & operator << (ostream &out, const vec3& v)
	{
		out << v.x << " " << v.y << " " << v.z;
		//out << v.v[0] << v.v[1] << v.v[2];
		return out;
	}

	friend vec3 operator *(const float p, const vec3& v)
	{
#ifdef USE_SIMD
		__m128 div = _mm_set_ps1(p);
		vec3 tmp;
		tmp.m128 = _mm_mul_ps(v.m128, div);
#else
		vec3 tmp(v.x*p, v.y*p, v.z*p);
#endif // USE_SIMD
		return tmp;
	}

	inline float lengthsq()const
	{
#ifdef USE_SIMD
		vec3 tmp;
		tmp.m128 = _mm_mul_ps(m128, m128);
		return tmp.x + tmp.y + tmp.z;
#else
		return x * x + y * y + z * z;
#endif // USE_SIMD
	}

	inline float length()const
	{
		return sqrt(lengthsq());
	}

	inline vec3 normalize() const
	{
#ifdef USE_SIMD
		vec3 tmp;
		tmp.m128 = _mm_set_ps1(length());
		tmp.m128 = _mm_div_ps(m128, tmp.m128);
#else
		vec3 tmp(x / length(), y / length(), z / length());
#endif // USE_SIMD
		return tmp;
	}

	inline float dot(const vec3& v)const
	{
#ifdef USE_SIMD
		__m128 tmp = _mm_setzero_ps();
		tmp = _mm_add_ps(tmp, _mm_mul_ps(m128, v.m128));
		return tmp.m128_f32[0] + tmp.m128_f32[1] + tmp.m128_f32[2] + tmp.m128_f32[3];

#else
		return x * v.x + y * v.y + z * v.z;
#endif // USE_SIMD
	}
	/*
	x		x1
	y		y1
	z		z1
	y*z1 -y1*z , z*x1-x*z1 , x*y1-y*x1
	*/

	inline vec3 cross(const vec3& v)const
	{
		return vec3(y*v.z - v.y*z, z*v.x - x * v.z, x*v.y - y *v.x);
	}

	static vec3 cross(const vec3& lhs, const vec3& rhs)
	{
		return vec3(lhs.y*rhs.z - rhs.y*lhs.z, lhs.z*rhs.x - lhs.x * rhs.z, lhs.x*rhs.y - lhs.y * rhs.x);
	}

	inline float distance(const vec3& v)const
	{
		vec3 tmp(v.x - x, v.y - y, v.z - z);
		return sqrt(tmp.dot(tmp));
	}

	static float distance(const vec3& lhs, const vec3& rhs)
	{
		vec3 tmp(rhs.x - lhs.x, rhs.y - lhs.y, rhs.z - lhs.z);
		return sqrt(tmp.dot(tmp));
	}

	inline float angle(const vec3& v)const
	{
		float value = this->normalize().dot(v.normalize());

		if (value < -1.0f)return acos(-1)*RAD2DEG;
		else if (value > 1.0f)return acos(1)*RAD2DEG;
		else return acos(value)*RAD2DEG;
	}

	static float angle(const vec3& lhs, const vec3& rhs)
	{
		float value = lhs.normalize().dot(rhs.normalize());

		if (value < -1.0f)return acos(-1)*RAD2DEG;
		else if (value > 1.0f)return acos(1)*RAD2DEG;
		else return acos(value)*RAD2DEG;
	}
};

ALIGN struct vec4
{
public:
	union
	{
		ALIGN float m[4];
		ALIGN struct { float x, y, z, w; };
		ALIGN struct { float r, g, b, a; };
		ALIGN struct {
			vec3 xyz;
		};
		ALIGN struct {
			vec3 rgb;
		};
		ALIGN struct {
			vec2 xy;
			vec2 zw;
		};

#ifdef USE_SIMD
		ALIGN __m128 m128;
#endif // USE_SIMD
	};



	vec4() {}
	vec4& operator = (const vec4& v)
	{
		if (this == &v)
		{
			return *this;
		}
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	vec4(const float p)
	{
#ifdef USE_SIMD
		m128 = _mm_set_ps1(p);
#else
		x = p;
		y = p;
		z = p;
		w = p;
#endif // USE_SIMD
	}

	vec4(const float px, const float py, const float pz, const float pw)
	{
#ifdef USE_SIMD
		m128 = _mm_set_ps(pw, pz, py, px);
#else
		x = px;
		y = py;
		z = pz;
		w = pw;
#endif // USE_SIMD
	}

	vec4(const vec3 &v, const float pw = 1)
	{
#ifdef USE_SIMD
		m128 = _mm_set_ps(pw, v.z, v.y, v.x);
#else
		x = v.x;
		y = v.y;
		z = v.z;
		w = pw;
#endif // USE_SIMD
	}

	inline vec4 & operator +=(const vec4& v)
	{
#ifdef USE_SIMD
		m128 = _mm_add_ps(m128, v.m128);
#else
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
#endif // USE_SIMD
		return *this;
	}

	inline vec4& operator -=(const vec4 &v)
	{
#ifdef USE_SIMD
		m128 = _mm_sub_ps(m128, v.m128);
#else
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
#endif // USE_SIMD
		return *this;
	}

	inline vec4& operator *=(const float p)
	{
#ifdef USE_SIMD
		__m128 div = _mm_set_ps1(p);
		m128 = _mm_mul_ps(m128, div);
#else
		x *= p;
		y *= p;
		z *= p;
		w *= p;
#endif // USE_SIMD
		return *this;
	}


	inline vec4& operator /= (const float p)
	{
#ifdef USE_SIMD
		__m128 div = _mm_set_ps1(p);
		m128 = _mm_div_ps(m128, div);
#else
		x /= p;
		y /= p;
		z /= p;
		w /= p;
#endif // USE_SIMD
		return *this;
	}

	inline vec4 operator +(const vec4& v)const
	{
#ifdef USE_SIMD
		vec4 tmp;
		tmp.m128 = _mm_add_ps(m128, v.m128);
#else
		vec4 tmp(x + v.x, y + v.y, z + v.z, w + v.w);
#endif // USE_SIMD
		return tmp;
	}

	inline vec4 operator -(const vec4& v)const
	{
#ifdef USE_SIMD
		vec4 tmp;
		tmp.m128 = _mm_sub_ps(m128, v.m128);
#else
		vec4 tmp(x - v.x, y - v.y, z - v.z, w - v.w);
#endif // USE_SIMD
		return tmp;
	}

	inline vec4 operator *(const float p)const
	{
#ifdef USE_SIMD
		vec4 tmp;

		tmp.m128 = _mm_mul_ps(m128, _mm_set_ps1(p));
#else
		vec4 tmp(x*p, y*p, z*p, w*p);
#endif // USE_SIMD
		return tmp;
	}

	inline vec4 operator * (const vec4& v) const
	{
		return vec4(x*v.x, y*v.y, z*v.z, w*v.w);
	}

	inline vec4 operator / (const float p)const
	{
#ifdef USE_SIMD
		vec4 tmp;
		tmp.m128 = _mm_set_ps1(p);
		tmp.m128 = _mm_div_ps(m128, tmp.m128);
#else
		vec4 tmp(x / p, y / p, z / p, w / p);
#endif // USE_SIMD
		return tmp;
	}

	friend ostream & operator << (ostream &out, const vec4& v)
	{
		out << v.x << " " << v.y << " " << v.z << " " << v.w;
		//out << v.v[0] << v.v[1] << v.v[2];
		return out;
	}

	bool operator == (const vec4& v)const
	{
		if (abs(x - v.x) < EQUAL_PRECISION&&abs(y - v.y) < EQUAL_PRECISION&&abs(z - v.z) < EQUAL_PRECISION&&abs(w - v.w) < EQUAL_PRECISION)
			return true;
		return false;
	}

	bool operator != (const vec4& v)const
	{
		if (abs(x - v.x) > EQUAL_PRECISION || abs(y - v.y) > EQUAL_PRECISION || abs(z - v.z) > EQUAL_PRECISION || abs(w - v.w) > EQUAL_PRECISION)
			return true;
		return false;
	}


	vec4 colorDot(const vec4 & a)
	{
		return vec4(x*a.x, y*a.y, z*a.z, w*a.w);
	}

	inline float lengthsq()const
	{
		return x * x + y * y + z * z + w * w;
	}

	inline float rLength()
	{
#ifdef USE_SIMD
		__m128 temp = m128;
		temp = _mm_mul_ps(temp, temp);
		temp = _mm_hadd_ps(temp, temp);
		temp = _mm_hadd_ps(temp, temp);
		temp = _mm_rsqrt_ps(temp);
		return temp.m128_f32[0];
#else
		return 1.0 / sqrtf(lengthsq());
#endif // USE_SIMD
	}

	inline float length()
	{
#ifdef USE_SIMD
		__m128 temp = m128;
		temp = _mm_mul_ps(temp, temp);
		temp = _mm_hadd_ps(temp, temp);
		temp = _mm_hadd_ps(temp, temp);
		temp = _mm_sqrt_ps(temp);
		return temp.m128_f32[0];
#else
		return sqrtf(lengthsq());
#endif // USE_SIMD
	}


	inline vec4 normalize()
	{
		vec4 tmp;
		float len;
#ifdef USE_SIMD
		len = rLength();
		tmp.m128 = _mm_mul_ps(m128, _mm_set_ps1(len));
#else
		len = length();
		tmp.x = x / len;
		tmp.y = y / len;
		tmp.z = z / len;
		tmp.w = w / len;
#endif // USE_SIMD
		return tmp;
	}

	inline float dot(const vec4& v)
	{
#ifdef USE_SIMD
		__m128 temp = _mm_mul_ps(m128, v.m128);
		return temp.m128_f32[0] + temp.m128_f32[1] + temp.m128_f32[2] + temp.m128_f32[3];
		//return *((float*)&temp) + *((float*)&temp + 1) + *((float*)&temp + 2) + *((float*)&temp + 3);
#else
		return x * v.x + y * v.y + z * v.z + w * v.w;
#endif // USE_SIMD
	}

	static inline float dot(const vec4& lhs, const vec4& rhs)
	{
#ifdef USE_SIMD
		__m128 tmp;
		tmp = _mm_mul_ps(lhs.m128, rhs.m128);
		return *((float*)&tmp) + *((float*)&tmp + 1) + *((float*)&tmp + 2) + *((float*)&tmp + 3);
		return 0;
#else
		return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w;
#endif // USE_SIMD
	}

};

#endif 