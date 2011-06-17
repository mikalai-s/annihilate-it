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