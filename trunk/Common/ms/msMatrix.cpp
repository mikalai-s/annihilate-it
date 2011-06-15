#include "msMatrix.h"

#include <memory.h>


msMatrix::msMatrix(void)
{
}


msMatrix::~msMatrix(void)
{
}



#define PI 3.1415926535897932384626433832795f

void msMatrix::scale(float sx, float sy, float sz)
{
	m_value[0][0] *= sx;
	m_value[0][1] *= sx;
	m_value[0][2] *= sx;
	m_value[0][3] *= sx;

	m_value[1][0] *= sy;
	m_value[1][1] *= sy;
	m_value[1][2] *= sy;
	m_value[1][3] *= sy;

	m_value[2][0] *= sz;
	m_value[2][1] *= sz;
	m_value[2][2] *= sz;
	m_value[2][3] *= sz;
}

void msMatrix::translate(float tx, float ty, float tz)
{
	m_value[3][0] += (m_value[0][0] * tx + m_value[1][0] * ty + m_value[2][0] * tz);
	m_value[3][1] += (m_value[0][1] * tx + m_value[1][1] * ty + m_value[2][1] * tz);
	m_value[3][2] += (m_value[0][2] * tx + m_value[1][2] * ty + m_value[2][2] * tz);
	m_value[3][3] += (m_value[0][3] * tx + m_value[1][3] * ty + m_value[2][3] * tz);
}

void msMatrix::rotate(float angle, float x, float y, float z)
{
	float sinAngle, cosAngle;
	float mag = sqrtf(x * x + y * y + z * z);

	sinAngle = sinf ( angle * PI / 180.0f );
	cosAngle = cosf ( angle * PI / 180.0f );
	if ( mag > 0.0f )
	{
		float xx, yy, zz, xy, yz, zx, xs, ys, zs;
		float oneMinusCos;
		msMatrix rotMat;

		x /= mag;
		y /= mag;
		z /= mag;

		xx = x * x;
		yy = y * y;
		zz = z * z;
		xy = x * y;
		yz = y * z;
		zx = z * x;
		xs = x * sinAngle;
		ys = y * sinAngle;
		zs = z * sinAngle;
		oneMinusCos = 1.0f - cosAngle;

		rotMat.m_value[0][0] = (oneMinusCos * xx) + cosAngle;
		rotMat.m_value[0][1] = (oneMinusCos * xy) - zs;
		rotMat.m_value[0][2] = (oneMinusCos * zx) + ys;
		rotMat.m_value[0][3] = 0.0F; 

		rotMat.m_value[1][0] = (oneMinusCos * xy) + zs;
		rotMat.m_value[1][1] = (oneMinusCos * yy) + cosAngle;
		rotMat.m_value[1][2] = (oneMinusCos * yz) - xs;
		rotMat.m_value[1][3] = 0.0F;

		rotMat.m_value[2][0] = (oneMinusCos * zx) - ys;
		rotMat.m_value[2][1] = (oneMinusCos * yz) + xs;
		rotMat.m_value[2][2] = (oneMinusCos * zz) + cosAngle;
		rotMat.m_value[2][3] = 0.0F; 

		rotMat.m_value[3][0] = 0.0F;
		rotMat.m_value[3][1] = 0.0F;
		rotMat.m_value[3][2] = 0.0F;
		rotMat.m_value[3][3] = 1.0F;

		multiply( &rotMat, this );
	}
}

void msMatrix::frustum(float left, float right, float bottom, float top, float nearZ, float farZ)
{
	float       deltaX = right - left;
	float       deltaY = top - bottom;
	float       deltaZ = farZ - nearZ;
	msMatrix    frust;

	if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
		(deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
		return;

	frust.m_value[0][0] = 2.0f * nearZ / deltaX;
	frust.m_value[0][1] = frust.m_value[0][2] = frust.m_value[0][3] = 0.0f;

	frust.m_value[1][1] = 2.0f * nearZ / deltaY;
	frust.m_value[1][0] = frust.m_value[1][2] = frust.m_value[1][3] = 0.0f;

	frust.m_value[2][0] = (right + left) / deltaX;
	frust.m_value[2][1] = (top + bottom) / deltaY;
	frust.m_value[2][2] = -(nearZ + farZ) / deltaZ;
	frust.m_value[2][3] = -1.0f;

	frust.m_value[3][2] = -2.0f * nearZ * farZ / deltaZ;
	frust.m_value[3][0] = frust.m_value[3][1] = frust.m_value[3][3] = 0.0f;

	multiply(&frust, this);
}


void msMatrix::perspective(float fovy, float aspect, float nearZ, float farZ)
{
	float frustumW, frustumH;

	frustumH = tanf( fovy / 360.0f * PI ) * nearZ;
	frustumW = frustumH * aspect;

	frustum( -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
}

void msMatrix::ortho(float left, float right, float bottom, float top, float nearZ, float farZ)
{
	float       deltaX = right - left;
	float       deltaY = top - bottom;
	float       deltaZ = farZ - nearZ;

	if ( (deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f) )
		return;

	msMatrix    ortho;
	ortho.identity();

	ortho.m_value[0][0] = 2.0f / deltaX;
	ortho.m_value[3][0] = -(right + left) / deltaX;
	ortho.m_value[1][1] = 2.0f / deltaY;
	ortho.m_value[3][1] = -(top + bottom) / deltaY;
	ortho.m_value[2][2] = -2.0f / deltaZ;
	ortho.m_value[3][2] = -(nearZ + farZ) / deltaZ;

	multiply(&ortho, this);
}


void msMatrix::multiply(msMatrix *srcA, msMatrix *srcB)
{
	msMatrix    tmp;
	int         i;

	for (i=0; i<4; i++)
	{
		tmp.m_value[i][0] =	(srcA->m_value[i][0] * srcB->m_value[0][0]) +
			(srcA->m_value[i][1] * srcB->m_value[1][0]) +
			(srcA->m_value[i][2] * srcB->m_value[2][0]) +
			(srcA->m_value[i][3] * srcB->m_value[3][0]) ;

		tmp.m_value[i][1] =	(srcA->m_value[i][0] * srcB->m_value[0][1]) + 
			(srcA->m_value[i][1] * srcB->m_value[1][1]) +
			(srcA->m_value[i][2] * srcB->m_value[2][1]) +
			(srcA->m_value[i][3] * srcB->m_value[3][1]) ;

		tmp.m_value[i][2] =	(srcA->m_value[i][0] * srcB->m_value[0][2]) + 
			(srcA->m_value[i][1] * srcB->m_value[1][2]) +
			(srcA->m_value[i][2] * srcB->m_value[2][2]) +
			(srcA->m_value[i][3] * srcB->m_value[3][2]) ;

		tmp.m_value[i][3] =	(srcA->m_value[i][0] * srcB->m_value[0][3]) + 
			(srcA->m_value[i][1] * srcB->m_value[1][3]) +
			(srcA->m_value[i][2] * srcB->m_value[2][3]) +
			(srcA->m_value[i][3] * srcB->m_value[3][3]) ;
	}
	memcpy(m_value, tmp.m_value, sizeof(float) * 4 * 4);
}


void msMatrix::identity()
{
	memset(m_value, 0x0, sizeof(msMatrix));
	m_value[0][0] = 1.0f;
	m_value[1][1] = 1.0f;
	m_value[2][2] = 1.0f;
	m_value[3][3] = 1.0f;
}

