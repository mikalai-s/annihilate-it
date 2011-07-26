#include "msMatrix.h"

#include <memory.h>


msMatrix::msMatrix(void)
{
}


msMatrix::msMatrix(float v[4][4])
{
    memcpy(this->value, v, sizeof(float) * 16);
}


msMatrix::~msMatrix(void)
{
}


void msMatrix::multiply(msMatrix &m)
{
    msMatrix tmp;
    for (int i = 0; i < 4; i ++)
    {
        tmp.value[i][0] =    
            (this->value[i][0] * m.value[0][0]) + (this->value[i][1] * m.value[1][0]) + (this->value[i][2] * m.value[2][0]) + (this->value[i][3] * m.value[3][0]) ;
        tmp.value[i][1] =    
            (this->value[i][0] * m.value[0][1]) + (this->value[i][1] * m.value[1][1]) + (this->value[i][2] * m.value[2][1]) + (this->value[i][3] * m.value[3][1]) ;
        tmp.value[i][2] =    
            (this->value[i][0] * m.value[0][2]) + (this->value[i][1] * m.value[1][2]) + (this->value[i][2] * m.value[2][2]) + (this->value[i][3] * m.value[3][2]) ;
        tmp.value[i][3] =    
            (this->value[i][0] * m.value[0][3]) + (this->value[i][1] * m.value[1][3]) + (this->value[i][2] * m.value[2][3]) + (this->value[i][3] * m.value[3][3]) ;
    }
    memcpy(this->value, tmp.value, sizeof(float) * 16);
}

msPoint3f msMatrix::multiply(msPoint3f &p)
{
    return msPoint3f(
        p.x * this->value[0][0] + p.y * this->value[1][0] + p.z * this->value[2][0] + this->value[3][0],
        p.x * this->value[0][1] + p.y * this->value[1][1] + p.z * this->value[2][1] + this->value[3][1],
        p.x * this->value[0][2] + p.y * this->value[1][2] + p.z * this->value[2][1] + this->value[3][2]);
}


msMatrix msMatrix::identity()
{
    msMatrix identity;
    memset(identity.value, 0, sizeof(msMatrix));
    identity.value[0][0] = 1.0f;
    identity.value[1][1] = 1.0f;
    identity.value[2][2] = 1.0f;
    identity.value[3][3] = 1.0f;
    return identity;
}