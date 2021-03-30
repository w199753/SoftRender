#pragma once
#ifndef __MAT__H__
#define __MAT__H__
/*
说明：
默认使用行向量填充,写了一坨屎，simd也是负优化······ 也不知道自己写这东西干啥
*/
#include"vec.h"
#include"math_def.h"

ALIGN struct mat3x3
{
public:
	union
	{
		float _m[3][3];
		float m[9];
		ALIGN struct
		{
			float m00, m01, m02, m10, m11, m12, m20, m21, m22;
		};


	};

	mat3x3() {}
	mat3x3(const vec3 &v1, const vec3& v2, const vec3& v3)
	{
		m00 = v1.x; m01 = v2.x; m02 = v3.x;
		m10 = v1.y; m11 = v2.y; m12 = v3.y;
		m20 = v1.z; m21 = v2.z; m22 = v3.z;
	}
	mat3x3(const float p00, const float p01, const float p02,
		const float p10, const float p11, const float p12,
		const float p20, const float p21, const float p22)
	{
		m[0] = p00; m[1] = p01; m[2] = p02;
		m[3] = p10; m[4] = p11; m[5] = p12;
		m[6] = p20; m[7] = p21; m[8] = p22;
	}

	const float * operator [](const int p) const
	{
		return _m[p];
	}

	mat3x3& operator +=(const mat3x3 & p)
	{
		for (register int i = 0; i < 9; i++)
			m[i] += p.m[i];
		return *this;
	}

	mat3x3& operator -=(const mat3x3 & p)
	{
		for (register int i = 0; i < 9; i++)
			m[i] -= p.m[i];
		return *this;
	}

	mat3x3& operator *=(const mat3x3& p)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				float sum = 0;
				for (int k = 0; k < 3; k++)
				{
					sum += _m[i][k] * p._m[k][j];
				}
				_m[i][j] = sum;
			}
		}
		return *this;
	}

	mat3x3 operator + (const mat3x3& p) const
	{
		mat3x3 tmp;
		for (register int i = 0; i < 9; i++)
			tmp.m[i] = m[i] + p.m[i];
		return tmp;
	}

	mat3x3 operator - (const mat3x3 & p) const
	{
		mat3x3 tmp;
		for (register int i = 0; i < 9; i++)
			tmp.m[i] = m[i] - p.m[i];
		return tmp;
	}

	mat3x3 operator *(const mat3x3 & p)const
	{
		mat3x3 res;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				float sum = 0;
				for (int k = 0; k < 3; k++)
				{
					sum += _m[i][k] * p._m[k][j];
				}
				res._m[i][j] = sum;
			}
		}
		return res;
	}

	vec3 operator*(const vec3& p)const
	{
		vec3 tmp(_m[0][0] * p.x + _m[0][1] * p.y + _m[0][2] * p.z,
			_m[1][0] * p.x + _m[1][1] * p.y + _m[1][2] * p.z,
			_m[2][0] * p.x + _m[2][1] * p.y + _m[2][2] * p.z);
		return tmp;
	}

	//转置
	mat3x3 Transpose()const
	{
		mat3x3 tmp;
		tmp.m00 = m00;		tmp.m01 = m10;		tmp.m02 = m20;
		tmp.m10 = m01;		tmp.m11 = m11;		tmp.m12 = m21;
		tmp.m20 = m02;		tmp.m21 = m12;		tmp.m22 = m22;
		return tmp;
	}

	//逆矩阵
	mat3x3 Inverse()const
	{
		mat3x3 res;
		//高斯消元法，处理多维方阵，但速度肯定没有那么快
		const int row = 3;
		float tmp[row][row * 2];
		//1.构建增广矩阵
		for (int i = 0;i < row;i++)
		{
		for (int j = 0;j < row * 2;j++)
		{
		if (j >= row)
		{
		tmp[i][j] = j == (row + i) ? 1 : 0;
		}
		else
		{
		tmp[i][j] = _m[i][j];
		}
		}
		}
		//2.化为行阶梯形式
		//特殊处理第一行
		for (int i = 1;i < row;i++)
		{
		for (int z = i;z < row;z++)
		{
		float k = (float)tmp[z][i - 1] / (float)tmp[i - 1][i - 1];
		//cout << k <<tmp[z][i-1]<<" "<<tmp[i-1][i-1] << " "<<i<< endl;
		tmp[z][i - 1] = 0;
		for (int j = i;j < row * 2;j++)
		{
		tmp[z][j] -= k * tmp[i - 1][j];
		}
		}
		}
		//对角线元素归1
		for (int i = 0;i < row;i++)
		{
		if (abs(tmp[i][i] - 1) >= EQUAL_PRECISION)
		{
		float k = tmp[i][i];
		for (int z = 0;z < row * 2;z++)
		{
		tmp[i][z] /= k;
		}
		}
		}

		//3.化为单位矩阵形式
		for (int i = row - 1;i >= 1;i--)
		{
		for (int z = i;z >= 1;z--)
		{
		float k = tmp[z - 1][i] / tmp[i][i];
		for (int j = i + 1;j < row * 2;j++)
		{
		tmp[z - 1][j] -= k * tmp[i][j];
		}
		}
		}

		//4.完成并输出
		for (int i = 0;i < row;i++)
		{
		for (int j = 0;j < row;j++)
		{
		res._m[i][j] = tmp[i][row + j];
		//cout << res._m[i][j] << " ";
		}
		//cout << endl;
		}
		

	
		return res;
	}

	//行列式
	float Determinant()const
	{

	}


	friend ostream& operator<< (ostream& output, const mat3x3& p)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				output << p._m[i][j] << " ";
			output << endl;
		}
		return output;
	}

};


ALIGN struct mat4x4
{
private:
	inline mat4x4 sse_mul(const mat4x4& rhs)const
	{
#ifdef USE_SIMD
		return mat4x4(_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(this->line1.m128_f32[0]), rhs.line1), _mm_mul_ps(_mm_set1_ps(this->line1.m128_f32[1]), rhs.line2)), _mm_mul_ps(_mm_set1_ps(this->line1.m128_f32[2]), rhs.line3)), _mm_mul_ps(_mm_set1_ps(this->line1.m128_f32[3]), rhs.line4)),
			_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(this->line2.m128_f32[0]), rhs.line1), _mm_mul_ps(_mm_set1_ps(this->line2.m128_f32[1]), rhs.line2)), _mm_mul_ps(_mm_set1_ps(this->line2.m128_f32[2]), rhs.line3)), _mm_mul_ps(_mm_set1_ps(this->line2.m128_f32[3]), rhs.line4)),
			_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(this->line3.m128_f32[0]), rhs.line1), _mm_mul_ps(_mm_set1_ps(this->line3.m128_f32[1]), rhs.line2)), _mm_mul_ps(_mm_set1_ps(this->line3.m128_f32[2]), rhs.line3)), _mm_mul_ps(_mm_set1_ps(this->line3.m128_f32[3]), rhs.line4)),
			_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(this->line4.m128_f32[0]), rhs.line1), _mm_mul_ps(_mm_set1_ps(this->line4.m128_f32[1]), rhs.line2)), _mm_mul_ps(_mm_set1_ps(this->line4.m128_f32[2]), rhs.line3)), _mm_mul_ps(_mm_set1_ps(this->line4.m128_f32[3]), rhs.line4))
		);
#endif // USE_SIMD
	}

public:
	ALIGN union
	{
		ALIGN float _m[4][4];
		ALIGN float m[16];
		ALIGN struct
		{
			float m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;
		};
		ALIGN struct
		{
			vec4 v1, v2, v3, v4;
		};

#ifdef USE_SIMD
		ALIGN struct
		{
			__m128 line1, line2, line3, line4;
		};
#endif // USE_SIMD
	};

#ifdef USE_SIMD
	mat4x4(__m128 r0, __m128 r1, __m128 r2, __m128 r3)
	{
		line1 = r0;
		line2 = r1;
		line3 = r2;
		line4 = r3;
	}
#endif // USE_SIMD

	mat4x4() {}
	mat4x4& operator = (const mat4x4& _m)
	{
		if (this == &_m)
			return *this;
		m00 = _m.m00;
		m01 = _m.m01;
		m02 = _m.m02;
		m03 = _m.m03;
		m10 = _m.m10;
		m11 = _m.m11;
		m12 = _m.m12;
		m13 = _m.m13;
		m20 = _m.m20;
		m21 = _m.m21;
		m22 = _m.m22;
		m23 = _m.m23;
		m30 = _m.m30;
		m31 = _m.m31;
		m32 = _m.m32;
		m33 = _m.m33;
		return *this;
	}
	mat4x4(const float p)
	{
#ifdef USE_SIMD
		line1 = _mm_set_ps(0, 0, 0, p);
		line2 = _mm_set_ps(0, 0, p, 0);
		line3 = _mm_set_ps(0, p, 0, 0);
		line4 = _mm_set_ps(p, 0, 0, 0);
		//v1 = vec4(p, 0, 0, 0);
		//v2 = vec4(0, p, 0, 0);
		//v3 = vec4(0, 0, p, 0);
		//v4 = vec4(0, 0, 0, p);
#else
		*(_m[0]) = p; *(_m[0] + 1) = 0; *(_m[0] + 2) = 0; *(_m[0] + 3) = 0;
		*(_m[1]) = 0; *(_m[1] + 1) = p; *(_m[1] + 2) = 0; *(_m[1] + 3) = 0;
		*(_m[2]) = 0; *(_m[2] + 1) = 0; *(_m[2] + 2) = p; *(_m[2] + 3) = 0;
		*(_m[3]) = 0; *(_m[3] + 1) = 0; *(_m[3] + 2) = 0; *(_m[3] + 3) = p;
#endif // USE_SIME
	}

	mat4x4(const vec4& v1, const vec4& v2, const vec4& v3, const vec4& v4)
	{
#ifdef USE_SIMD
		/*line1 = v1.m128;
		line2 = v2.m128;
		line3 = v3.m128;
		line4 = v4.m128;*/
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
		this->v4 = v4;
#else
		*(_m[0]) = v1.x; *(_m[0] + 1) = v1.y; *(_m[0] + 2) = v1.z; *(_m[0] + 3) = v1.w;
		*(_m[1]) = v2.x; *(_m[1] + 1) = v2.y; *(_m[1] + 2) = v2.z; *(_m[1] + 3) = v2.w;
		*(_m[2]) = v3.x; *(_m[2] + 1) = v3.y; *(_m[2] + 2) = v3.z; *(_m[2] + 3) = v3.w;
		*(_m[3]) = v4.x; *(_m[3] + 1) = v4.y; *(_m[3] + 2) = v4.z; *(_m[3] + 3) = v4.w;
#endif // USE_SIMD
	}

	mat4x4(const float p00, const float p01, const float p02, const float p03,
		const float p10, const float p11, const float p12, const float p13,
		const float p20, const float p21, const float p22, const float p23,
		const float p30, const float p31, const float p32, const float p33)
	{
#ifdef USE_SIMD
		line1 = _mm_set_ps(p03, p02, p01, p00);
		line2 = _mm_set_ps(p13, p12, p11, p10);
		line3 = _mm_set_ps(p23, p22, p21, p20);
		line4 = _mm_set_ps(p33, p32, p31, p30);
#else
		*(_m[0]) = m00; *(_m[0] + 1) = m01; *(_m[0] + 2) = m02; *(_m[0] + 3) = m03;
		*(_m[1]) = m10; *(_m[1] + 1) = m11; *(_m[1] + 2) = m12; *(_m[1] + 3) = m13;
		*(_m[2]) = m20; *(_m[2] + 1) = m21; *(_m[2] + 2) = m22; *(_m[2] + 3) = m23;
		*(_m[3]) = m30; *(_m[3] + 1) = m31; *(_m[3] + 2) = m32; *(_m[3] + 3) = m33;
#endif // USE_SIME
	}

	mat4x4 & operator *=(const mat4x4& rhs)
	{

#ifdef USE_SIMD
		mat4x4 res = sse_mul(rhs);
		line1 = res.line1;
		line2 = res.line2;
		line3 = res.line3;
		line4 = res.line4;
#else
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float sum = 0;
				for (int k = 0; k < 4; k++)
				{
					sum += _m[i][k] * rhs._m[k][j];
				}
				_m[i][j] = sum;
			}
		}
#endif // USE_SIMD
		return *this;
	}

	vec4 operator *(const vec4& v)
	{
		vec4 res;
#ifdef USE_SIMD
		//临时写法，海洋能改进
		res.x = v1.dot(v);
		res.y = v2.dot(v);
		res.z = v3.dot(v);
		res.w = v4.dot(v);
#else
		res.x = v1.dot(v);
		res.y = v2.dot(v);
		res.y = v3.dot(v);
		res.y = v4.dot(v);
#endif // USE_SIMD
		return res;
	}

	vec4& operator *=(const vec4& v)
	{
#ifdef USE_SIMD

#else

#endif // USE_SIMD
	}

	/*
	1 1 1 1
	2 2 2 2
	3 3 3 3
	4 4 4 4
	*/
	mat4x4  operator *(mat4x4 &rhs)const
	{
		mat4x4 tmp;
#ifdef USE_SIMD
		//return mat4x4(
		tmp.line1 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(this->line1.m128_f32[0]), rhs.line1), _mm_mul_ps(_mm_set1_ps(this->line1.m128_f32[1]), rhs.line2)), _mm_mul_ps(_mm_set1_ps(this->line1.m128_f32[2]), rhs.line3)), _mm_mul_ps(_mm_set1_ps(this->line1.m128_f32[3]), rhs.line4));
		tmp.line2 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(this->line2.m128_f32[0]), rhs.line1), _mm_mul_ps(_mm_set1_ps(this->line2.m128_f32[1]), rhs.line2)), _mm_mul_ps(_mm_set1_ps(this->line2.m128_f32[2]), rhs.line3)), _mm_mul_ps(_mm_set1_ps(this->line2.m128_f32[3]), rhs.line4));
		tmp.line3 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(this->line3.m128_f32[0]), rhs.line1), _mm_mul_ps(_mm_set1_ps(this->line3.m128_f32[1]), rhs.line2)), _mm_mul_ps(_mm_set1_ps(this->line3.m128_f32[2]), rhs.line3)), _mm_mul_ps(_mm_set1_ps(this->line3.m128_f32[3]), rhs.line4));
		tmp.line4 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(this->line4.m128_f32[0]), rhs.line1), _mm_mul_ps(_mm_set1_ps(this->line4.m128_f32[1]), rhs.line2)), _mm_mul_ps(_mm_set1_ps(this->line4.m128_f32[2]), rhs.line3)), _mm_mul_ps(_mm_set1_ps(this->line4.m128_f32[3]), rhs.line4));

#else
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float sum = 0;
				for (int k = 0; k < 4; k++)
				{
					sum += _m[i][k] * rhs._m[k][j];
				}
				tmp._m[i][j] = sum;
			}
		}
#endif // USE_SIMD
		return tmp;
	}

	mat4x4 operator /(const float p)
	{
		mat4x4 tmp;
#ifdef USE_SIMD
		__m128 t = _mm_set1_ps(p);
		tmp.line1 = _mm_div_ps(line1, t);
		tmp.line2 = _mm_div_ps(line2, t);
		tmp.line3 = _mm_div_ps(line3, t);
		tmp.line4 = _mm_div_ps(line4, t);
#else
		for (int i = 0; i < 16; i++)
		{
			tmp.m[i] = m[i] / p;
		}
#endif
		return tmp;
	}

	mat4x4& operator +=(const mat4x4& p)
	{
#ifdef USE_SIMD
		line1 = _mm_add_ps(line1, p.line1);
		line2 = _mm_add_ps(line2, p.line2);
		line3 = _mm_add_ps(line3, p.line3);
		line4 = _mm_add_ps(line4, p.line4);
#else
		for (int i = 0; i < 16; i++)
		{
			m[i] += p.m[i];
		}
#endif
		return *this;
	}

	mat4x4 operator +(const mat4x4& p)const
	{
		mat4x4 tmp;
#ifdef USE_SIMD
		tmp.line1 = _mm_add_ps(line1, p.line1);
		tmp.line2 = _mm_add_ps(line2, p.line2);
		tmp.line3 = _mm_add_ps(line3, p.line3);
		tmp.line4 = _mm_add_ps(line4, p.line4);
#else
		for (int i = 0; i < 16; i++)
		{
			tmp.m[i] = m[i] + p.m[i];
		}
#endif
		return tmp;
	}

	mat4x4& operator -=(const mat4x4& p)
	{
#ifdef USE_SIMD
		line1 = _mm_sub_ps(line1, p.line1);
		line2 = _mm_sub_ps(line2, p.line2);
		line3 = _mm_sub_ps(line3, p.line3);
		line4 = _mm_sub_ps(line4, p.line4);
#else
		for (int i = 0; i < 16; i++)
		{
			m[i] -= p.m[i];
		}
#endif
		return *this;
	}

	mat4x4 operator - (const mat4x4& p)const
	{
		mat4x4 tmp;
#ifdef USE_SIMD
		tmp.line1 = _mm_sub_ps(line1, p.line1);
		tmp.line2 = _mm_sub_ps(line2, p.line2);
		tmp.line3 = _mm_sub_ps(line3, p.line3);
		tmp.line4 = _mm_sub_ps(line4, p.line4);
#else
		for (int i = 0; i < 16; i++)
		{
			tmp.m[i] = m[i] - p.m[i];
		}
#endif
		return tmp;
	}

	const float * operator [](const int p) const
	{
		return _m[p];
	}

	mat4x4 Transpose()const//转置已经是最快了
	{
		mat4x4 tmp;
		tmp.m00 = m00;		tmp.m01 = m10;		tmp.m02 = m20; tmp.m03 = m30;
		tmp.m10 = m01;		tmp.m11 = m11;		tmp.m12 = m21; tmp.m13 = m31;
		tmp.m20 = m02;		tmp.m21 = m12;		tmp.m22 = m22; tmp.m23 = m32;
		tmp.m30 = m03;		tmp.m31 = m13;		tmp.m32 = m23; tmp.m33 = m33;
		return tmp;
	}
	/*
	- - - -
	1 - 1 1
	1 - 1 1
	1 - 1 1
	*/
	mat4x4 Inverse()
	{
		mat4x4 tmp;
#ifdef USE_SIMD
		//虽然很笨，但为了效率我不得不这么做
		//tmp.m00 = m11*m22*m33 + m12*m23*m31 + m13*m21*m32 - (m13*m22*m31 + m23*m32*m11 + m33*m12*m21);
		//tmp.m02 = m
		//
		//tmp.m11 = m00*m22*m33 + m02*m23*m30 + m03*m20*m32 - (m03*m22*m30 + m23*m32*m00 + m33*m02*m20);
		return mat4x4(subDeterminant(0, 0), -subDeterminant(0, 1), subDeterminant(0, 2), -subDeterminant(0, 3),
			-subDeterminant(1, 0), subDeterminant(1, 1), -subDeterminant(1, 2), subDeterminant(1, 3),
			subDeterminant(2, 0), -subDeterminant(2, 1), subDeterminant(2, 2), -subDeterminant(2, 3),
			-subDeterminant(3, 0), subDeterminant(3, 1), -subDeterminant(3, 2), subDeterminant(3, 3)).Transpose() / Determinant();
#else
		return mat4x4(subDeterminant(0, 0), -subDeterminant(0, 1), subDeterminant(0, 2), -subDeterminant(0, 3),
			-subDeterminant(1, 0), subDeterminant(1, 1), -subDeterminant(1, 2), subDeterminant(1, 3),
			subDeterminant(2, 0), -subDeterminant(2, 1), subDeterminant(2, 2), -subDeterminant(2, 3),
			-subDeterminant(3, 0), subDeterminant(3, 1), -subDeterminant(3, 2), subDeterminant(3, 3)).Inverse / Determinant();
#endif
		return tmp;
	}

	float subDeterminant(int row, int column)const
	{
		float res = 0;
		float t[3][3];
		int iIndex = 0, jIndex = 0;
		for (int i = 0; i < 4; i++)
		{
			if (i == (row))continue;

			for (int j = 0; j < 4; j++)
			{
				if (j == (column))continue;
				t[iIndex][jIndex] = _m[i][j];
				jIndex++;
			}
			iIndex++;
			jIndex = 0;
		}

		//for (int i = 0; i < 3; i++)
		//{
		//	for (int j = 0; j < 3; j++)
		//	{
		//		cout << t[i][j] << " ";
		//	}
		//	cout << endl;
		//}
		res = (t[0][0] * t[1][1] * t[2][2] + t[0][1] * t[1][2] * t[2][0] + t[0][2] * t[1][0] * t[2][1])
			- (t[0][2] * t[1][1] * t[2][0] + t[0][1] * t[1][0] * t[2][2] + t[0][0] * t[1][2] * t[2][1]);
		return res;
	}


	inline float Determinant()const //行列式应该也是最快了
	{
		return m00 * m11*m22*m33 + m01 * m12*m23*m30 + m02 * m13*m31*m20 + m03 * m32*m21*m10 -
			(m03*m12*m21*m30 + m02 * m11*m20*m33 + m01 * m10*m32*m23 + m00 * m31*m22*m13);
	}


	friend ostream & operator <<(ostream & output, const mat4x4& p)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				output << p._m[i][j] << " ";
			output << endl;
		}
		return output;
	}

};

#endif // !__MAT__H__

