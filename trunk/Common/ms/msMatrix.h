#include <math.h>
#include "msCommon.h"

#pragma once
class msMatrix
{
	float m_value[4][4];

public:
	msMatrix(void);
	~msMatrix(void);

	msMatrix* scale(float sx, float sy, float sz);
	msMatrix* translate(float tx, float ty, float tz);
	msMatrix* rotate(float angle, float x, float y, float z);
	msMatrix* frustum(float left, float right, float bottom, float top, float nearZ, float farZ);
	msMatrix* perspective(float fovy, float aspect, float nearZ, float farZ);
	msMatrix* ortho(float left, float right, float bottom, float top, float nearZ, float farZ);
	msMatrix* multiply(msMatrix *srcB);
	msMatrix* identity();

	msPointf multiply(msPointf v);

	float* getArray() const
	{
		return (float*)m_value;
	}


};

