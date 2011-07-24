#include "msSpline.h"


msSpline::msSpline(void)
{
    m_resolution = 3;
    m_count = 0;
}


msSpline::~msSpline(void)
{
}

msPoint3f msSpline::Spline_Calc(msPoint3f Ap, msPoint3f Bp, msPoint3f Cp, msPoint3f Dp, float T, float D)
{
     float T2 = T * T;        // Square of t 
     float T3 = T2 * T;        // Cube of t 
     msPoint3f res(
        ((Ap.x*T3) + (Bp.x*T2) + (Cp.x*T) + Dp.x)/D,  // Calc x value 
        ((Ap.y*T3) + (Bp.y*T2) + (Cp.y*T) + Dp.y)/D);  // Calc y value 
     return res;
}

void msSpline::BSpline_ComputeCoeffs (int N, msPoint3f *Ap, msPoint3f *Bp, msPoint3f *Cp, msPoint3f *Dp)
{
    BSpline_ComputeCoeffs_Private(CtrlPt[N].x, CtrlPt[N+1].x, CtrlPt[N+2].x, CtrlPt[N+3].x,
        &Ap->x, &Bp->x, &Cp->x, &Dp->x);

    BSpline_ComputeCoeffs_Private(CtrlPt[N].y, CtrlPt[N+1].y, CtrlPt[N+2].y, CtrlPt[N+3].y,
        &Ap->y, &Bp->y, &Cp->y, &Dp->y);
}

void msSpline::BSpline_ComputeCoeffs_Private(float c0, float c1, float c2, float c3, float *a, float *b, float *c, float *d)
{
    *a = -c0 + 3.0f*c1 - 3.0f*c2 + c3;
    *b = 3.0f*c0 - 6.0f*c1 + 3.0f*c2;
    *c = -3.0f*c0 + 3.0f*c2;
    *d = c0 + 4.0f*c1 + c2;
}

void msSpline::draw()
{
    // duplicate two first points
    CtrlPt[0] = CtrlPt[2];
    CtrlPt[1] = CtrlPt[2];

    // close b-spline
    CtrlPt[m_count+2] = CtrlPt[2];
    CtrlPt[m_count+3] = CtrlPt[3];
    CtrlPt[m_count+4] = CtrlPt[4];

    // duplicate last two points
    CtrlPt[m_count+5] = CtrlPt[m_count+4];
    CtrlPt[m_count+6] = CtrlPt[m_count+4];

    msPoint3f Ap, Bp, Cp, Dp;

    for(int i = 2; i < m_count + 2; i ++)
    {
        BSpline_ComputeCoeffs(i, &Ap, &Bp, &Cp, &Dp);

        msPoint3f p1 = Spline_Calc(Ap, Bp, Cp, Dp, 0.0f, 6.0f);

        for(int j = 1; j < m_resolution + 1; j ++)
        {
            msPoint3f p2 = Spline_Calc(Ap, Bp, Cp, Dp, (float)j / (float)m_resolution, 6.0f);

            printf("(%f, %f)-(%f,%f)\r\n", p1.x, p1.y, p2.x, p2.y);

            p1 = p2;
        }
    }
}

void msSpline::getSplinePoints(int resolution, msPoint3f *points, int *count, int startIndex, bool loop)
{
    // duplicate two first points
    CtrlPt[0] = CtrlPt[2];
    CtrlPt[1] = CtrlPt[2];

    // close b-spline
    CtrlPt[m_count+2] = CtrlPt[2];
    CtrlPt[m_count+3] = CtrlPt[3];
    CtrlPt[m_count+4] = CtrlPt[4];

    // duplicate last two points
    CtrlPt[m_count+5] = CtrlPt[m_count+4];
    CtrlPt[m_count+6] = CtrlPt[m_count+4];

    msPoint3f Ap, Bp, Cp, Dp;

    int firstIndex = startIndex;
    *count = 0;

    for(int i = 2; i < m_count + 2; i ++)
    {
        BSpline_ComputeCoeffs(i, &Ap, &Bp, &Cp, &Dp);

        for(int j = 1; j < resolution; j ++)
        {
            points[startIndex + (*count)++] = Spline_Calc(Ap, Bp, Cp, Dp, (float)j / (float)resolution, 6.0f);
        }
    }

    if(loop)
        points[startIndex + (*count)++] = points[firstIndex];
}

void msSpline::addControlPoint(msPoint3f p)
{
    CtrlPt[m_count + 2] = p;
    m_count ++;
}

void msSpline::addControlPoint(float x, float y)
{
    addControlPoint(msPoint3f(x, y));
}

