#pragma once
#ifndef __MATH__DEF__H__
#define __MATH__DEF__H__

//����=�Ƕ�*pi/180
#define EQUAL_PRECISION 0.001f
#define PI 3.1415f
#define DEG2RAD 0.0174532924f
#define RAD2DEG 57.29578f

#define DIV_255 0.0039215686f

//�Ƿ���SIMD�Ż�
#define USE_SIMD


//�ڴ����
#ifdef OS_WIN32
#define ALIGN _CRT_ALIGN(16)
#else
#define ALIGN
#endif // OS_WIN32


#endif // __MATH__DEF__H__



