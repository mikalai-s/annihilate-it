#include "msMatrix.h"

#include <memory.h>


msMatrix::msMatrix(void)
{
}


msMatrix::msMatrix(float v[4][4])
{
    memcpy(m_value, v, sizeof(float) * 16);
}


msMatrix::~msMatrix(void)
{
}


void msMatrix::multiply(msMatrix &m)
{
    msMatrix tmp;
    for (int i = 0; i < 4; i ++)
    {
        tmp.m_value[i][0] =    
            (m_value[i][0] * m.m_value[0][0]) + (m_value[i][1] * m.m_value[1][0]) + (m_value[i][2] * m.m_value[2][0]) + (m_value[i][3] * m.m_value[3][0]) ;
        tmp.m_value[i][1] =    
            (m_value[i][0] * m.m_value[0][1]) + (m_value[i][1] * m.m_value[1][1]) + (m_value[i][2] * m.m_value[2][1]) + (m_value[i][3] * m.m_value[3][1]) ;
        tmp.m_value[i][2] =    
            (m_value[i][0] * m.m_value[0][2]) + (m_value[i][1] * m.m_value[1][2]) + (m_value[i][2] * m.m_value[2][2]) + (m_value[i][3] * m.m_value[3][2]) ;
        tmp.m_value[i][3] =    
            (m_value[i][0] * m.m_value[0][3]) + (m_value[i][1] * m.m_value[1][3]) + (m_value[i][2] * m.m_value[2][3]) + (m_value[i][3] * m.m_value[3][3]) ;
    }
    memcpy(this->m_value, tmp.m_value, sizeof(float) * 16);
}

msPoint3f msMatrix::multiply(msPoint3f &p)
{
    return msPoint3f(
        p.x * m_value[0][0] + p.y * m_value[1][0] + p.z * m_value[2][0] + m_value[3][0],
        p.x * m_value[0][1] + p.y * m_value[1][1] + p.z * m_value[2][1] + m_value[3][1],
        p.x * m_value[0][2] + p.y * m_value[1][2] + p.z * m_value[2][1] + m_value[3][2]);
}


msMatrix msMatrix::identity()
{
    msMatrix identity;
    memset(identity.m_value, 0, sizeof(msMatrix));
    identity.m_value[0][0] = 1.0f;
    identity.m_value[1][1] = 1.0f;
    identity.m_value[2][2] = 1.0f;
    identity.m_value[3][3] = 1.0f;
    return identity;
}