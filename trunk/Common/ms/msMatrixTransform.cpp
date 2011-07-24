#include "msMatrixTransform.h"

#include <memory.h>


msMatrixTransform::msMatrixTransform()
{
    m_matrix = new msMatrix();
    memcpy(m_matrix->m_value, msMatrix::identity().m_value, sizeof(float) * 16);
}

msMatrixTransform::msMatrixTransform(msMatrix &m)
{
    m_matrix = new msMatrix();
    memcpy(m_matrix->m_value, m.m_value, sizeof(float) * 16);
}

msMatrixTransform::~msMatrixTransform(void)
{
    delete m_matrix;
}

msMatrixTransform* msMatrixTransform::scale(float sx, float sy, float sz)
{
    msMatrix m = msMatrix::identity();

    m.m_value[0][0] *= sx;
    m.m_value[0][1] *= sx;
    m.m_value[0][2] *= sx;
    m.m_value[0][3] *= sx;

    m.m_value[1][0] *= sy;
    m.m_value[1][1] *= sy;
    m.m_value[1][2] *= sy;
    m.m_value[1][3] *= sy;

    m.m_value[2][0] *= sz;
    m.m_value[2][1] *= sz;
    m.m_value[2][2] *= sz;
    m.m_value[2][3] *= sz;

    m_matrix->multiply(m);

    return this;
}

msMatrixTransform* msMatrixTransform::translate(float tx, float ty, float tz)
{
    msMatrix m = msMatrix::identity();

    m.m_value[3][0] += (m.m_value[0][0] * tx + m.m_value[1][0] * ty + m.m_value[2][0] * tz);
    m.m_value[3][1] += (m.m_value[0][1] * tx + m.m_value[1][1] * ty + m.m_value[2][1] * tz);
    m.m_value[3][2] += (m.m_value[0][2] * tx + m.m_value[1][2] * ty + m.m_value[2][2] * tz);
    m.m_value[3][3] += (m.m_value[0][3] * tx + m.m_value[1][3] * ty + m.m_value[2][3] * tz);

    m_matrix->multiply(m);

    return this;
}

msMatrixTransform* msMatrixTransform::rotate(float angle, float x, float y, float z)
{
    float sinAngle, cosAngle;
    float mag = sqrtf(x * x + y * y + z * z);

    sinAngle = sinf ( angle );
    cosAngle = cosf ( angle );
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

        m_matrix->multiply( rotMat );
    }

    return this;
}

msMatrixTransform* msMatrixTransform::frustum(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = farZ - nearZ;
    msMatrix    frust;

    if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
        (deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
        return this;

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

    m_matrix->multiply(frust);

    return this;
}


msMatrixTransform* msMatrixTransform::ortho(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = farZ - nearZ;

    if ( (deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f) )
        return this;

    msMatrix ortho = msMatrix::identity();

    ortho.m_value[0][0] = 2.0f / deltaX;
    ortho.m_value[3][0] = -(right + left) / deltaX;
    ortho.m_value[1][1] = 2.0f / deltaY;
    ortho.m_value[3][1] = -(top + bottom) / deltaY;
    ortho.m_value[2][2] = -2.0f / deltaZ;
    ortho.m_value[3][2] = -(nearZ + farZ) / deltaZ;

    m_matrix->multiply(ortho);

    return this;
}


msMatrixTransform* msMatrixTransform::perspective(float fieldOfViewY, float aspectRatio, float zNearPlane, double zFarPlane)
{
    float height = 1.0f / tanf(fieldOfViewY / 2.0f);
    float width = height / aspectRatio;
    float d = zNearPlane - zFarPlane;

    float mv[4][4] = 
    {
        width, 0, 0, 0, 
        0, height, 0, 0,
        0, 0, zFarPlane / d, -1, 
        0, 0, zNearPlane * zFarPlane / d, 0
    };

    msMatrix m(mv);
    m_matrix->multiply(m);    

    return this;
}

msMatrixTransform* msMatrixTransform::viewport(float width, float height)
{
    float mv[4][4] = 
    {
        width / 2.0,    0.0,            0.0,    0.0,
        0.0,            -height / 2.0,  0.0,    0.0,
        0.0,            0.0,            1.0,    0.0,
        width / 2.0,    height / 2.0,   0.0,    1.0,
    };

    msMatrix m(mv);
    m_matrix->multiply(m);

    return this;
}

void msMatrixTransform::multiplyMatrix( msMatrix &matrix )
{
    m_matrix->multiply(matrix);
}



