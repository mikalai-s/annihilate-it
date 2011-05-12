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

	msPoint CtrlPt[80];

	void BSpline_ComputeCoeffs(int N, msPoint *Ap, msPoint *Bp, msPoint *Cp, msPoint *Dp);

	msPoint Spline_Calc(msPoint Ap, msPoint Bp, msPoint Cp, msPoint Dp, float T, float D);

	void draw();

	void addControlPoint(msPoint p);

	void addControlPoint(float x, float y);

	void getSplinePoints(int resolution, msPoint *points, int *count);
};