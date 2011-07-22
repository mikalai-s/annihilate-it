#include <math.h>
#include "msCommon.h"

#pragma once
struct msMatrix
{
	friend class msMatrixTransform;

	float m_value[4][4];

public:
    msMatrix(float v[4][4]);
	msMatrix(void);
	~msMatrix(void);
	
	void multiply(msMatrix &m);

    msPoint3f multiply(msPoint3f &p);

	static msMatrix identity();

	float* getArray() const
	{
		return (float*)m_value;
	}
};