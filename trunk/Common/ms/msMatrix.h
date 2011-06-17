#include <math.h>
#include "msCommon.h"

#pragma once
struct msMatrix
{
	friend class msMatrixTransform;

	float m_value[4][4];

public:
	msMatrix(void);
	~msMatrix(void);
	
	void multiply(msMatrix *m);

	static msMatrix identity();

	float* getArray() const
	{
		return (float*)m_value;
	}
};

class msMatrixTransform
{
	msMatrix m_matrix;

public:
	msMatrixTransform();
	msMatrixTransform(msMatrix &m);
	~msMatrixTransform(void);

	msMatrixTransform& scale(float sx, float sy, float sz);
	msMatrixTransform& translate(float tx, float ty, float tz);
	msMatrixTransform& rotate(float angle, float x, float y, float z);
	msMatrixTransform& frustum(float left, float right, float bottom, float top, float nearZ, float farZ);
	msMatrixTransform& perspective(float fovy, float aspect, float nearZ, float farZ);
	msMatrixTransform& ortho(float left, float right, float bottom, float top, float nearZ, float farZ);

	msMatrix& getMatrix() const
	{
		return (msMatrix)m_matrix;
	}
};

