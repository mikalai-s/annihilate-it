#include <math.h>

#pragma once
class msMatrix
{
	float m_value[4][4];

public:
	msMatrix(void);
	~msMatrix(void);

	void scale(float sx, float sy, float sz);
	void translate(float tx, float ty, float tz);
	void rotate(float angle, float x, float y, float z);
	void frustum(float left, float right, float bottom, float top, float nearZ, float farZ);
	void perspective(float fovy, float aspect, float nearZ, float farZ);
	void ortho(float left, float right, float bottom, float top, float nearZ, float farZ);
	void multiply(msMatrix *srcA, msMatrix *srcB);
	void identity();

	float* getArray() const
	{
		return (float*)m_value;
	}
};

