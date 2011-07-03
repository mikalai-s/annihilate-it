#include <math.h>
#include "msCommon.h"
#include "msMatrix.h"

#pragma once

class msMatrixTransform
{
	msMatrix *m_matrix;

public:
	msMatrixTransform();
	msMatrixTransform(msMatrix &m);
	~msMatrixTransform(void);

	msMatrixTransform* scale(float sx, float sy, float sz);
	msMatrixTransform* translate(float tx, float ty, float tz);
	msMatrixTransform* rotate(float angle, float x, float y, float z);
	msMatrixTransform* frustum(float left, float right, float bottom, float top, float nearZ, float farZ);
	msMatrixTransform* ortho(float left, float right, float bottom, float top, float nearZ, float farZ);

    msMatrixTransform* perspective(float fieldOfViewY, float aspectRatio, float zNearPlane, double zFarPlane);
    msMatrixTransform* viewport(float width, float height);

    void multiplyMatrix( msMatrix &matrix );

	msMatrix* getMatrix() const
	{
		return m_matrix;
	}

};

