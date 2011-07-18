#pragma once

#include "../msCommon.h"


class msSpline
{
	void BSpline_ComputeCoeffs_Private(float c0, float c1, float c2, float c3, float *a, float *b, float *c, float *d);

public:
	msSpline(void);
	~msSpline(void);

	int m_resolution;
	int m_count;

	//msPoints m_controlPoints;

	msPointf CtrlPt[80];

	void BSpline_ComputeCoeffs(int N, msPointf *Ap, msPointf *Bp, msPointf *Cp, msPointf *Dp);

	msPointf Spline_Calc(msPointf Ap, msPointf Bp, msPointf Cp, msPointf Dp, float T, float D);

	void draw();

	void addControlPoint(msPointf p);

	void addControlPoint(float x, float y);

	void getSplinePoints(int resolution, msPointf *points, int *count, int startIndex = 0, bool loop = false);
};