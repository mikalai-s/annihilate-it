#include "msUniform.h"
#include "msShaderProgram.h"

#define SAMPLE_FLOAT        1
#define SAMPLE_FLOAT_VEC2    2
#define SAMPLE_FLOAT_VEC3    3
#define SAMPLE_FLOAT_VEC4    4
#define SAMPLE_FLOAT_MAT4    5
#define SAMPLE_BOOL            6
#define SAMPLE_BOOL_VEC2    7
#define SAMPLE_BOOL_VEC3    8
#define SAMPLE_BOOL_VEC4    9

// constructor for uniform with no initial value
msUniform::msUniform(string &name)
{
    m_name = copyString(name);
    m_type = -1;
}

msUniform::msUniform(string &name, GLfloat v)
{
    m_name = copyString(name);

    m_floatData[0] = v;
    m_type = SAMPLE_FLOAT;
}

msUniform::msUniform(string &name, GLfloat v[], GLuint size)
{
    m_name = copyString(name);

    for(GLuint i = 0; i < size; i ++)
        m_floatData[i] = v[i];

    m_type = (size == 2) ? SAMPLE_FLOAT_VEC2 :
             (size == 3) ? SAMPLE_FLOAT_VEC3 :
             (size == 4) ? SAMPLE_FLOAT_VEC4 : 0;
}

msUniform::msUniform(string &name, GLfloat matrix[4][4])
{
    m_name = copyString(name);

    for(GLuint i = 0; i < 4; i ++)
        for(GLuint j = 0; j < 0; j ++)
            m_matrixData[i][j] = matrix[i][j];
    m_type = SAMPLE_FLOAT_MAT4;
}

msUniform::msUniform(string &name, GLboolean b)
{
    m_name = copyString(name);

    m_floatData[0] = b;
    m_type = SAMPLE_BOOL;
}

msUniform::msUniform(string &name, GLboolean b[], GLuint size)
{
    m_name = copyString(name);

    for(GLuint i = 0; i < size; i ++)
        m_boolData[i] = b[i];

    m_type = (size == 2) ? SAMPLE_BOOL_VEC2 :
             (size == 3) ? SAMPLE_BOOL_VEC3 :
             (size == 4) ? SAMPLE_BOOL_VEC4 : 0;
}


msUniform::~msUniform(void)
{
}

void msUniform::setProgram(msShaderProgram *program)
{
    m_program = program;
}

void msUniform::link()
{
    m_location = glGetUniformLocation( m_program->getHandle(), m_name);

    if(m_type == SAMPLE_FLOAT)
    {
        set1f(m_floatData[0]);
    }
    else if (m_type == SAMPLE_FLOAT_VEC2)
    {
        set2f(m_floatData[0], m_floatData[1]);
    }
    else if (m_type == SAMPLE_FLOAT_VEC3)
    {
        set3f(m_floatData[0], m_floatData[1], m_floatData[2]);
    }
    else if (m_type == SAMPLE_FLOAT_VEC4)
    {
        set4f(m_floatData[0], m_floatData[1], m_floatData[2], m_floatData[3]);
    }
    else if (m_type == SAMPLE_FLOAT_MAT4)
    {
        setMatrix4fv(1, GL_FALSE, m_floatData);
    }
    else if(m_type == SAMPLE_BOOL)
    {
        set1i(m_boolData[0]);
    }
    else if (m_type == SAMPLE_BOOL_VEC2)
    {
        set2i(m_boolData[0], m_boolData[1]);
    }
    else if (m_type == SAMPLE_BOOL_VEC3)
    {
        set3i(m_boolData[0], m_boolData[1], m_boolData[2]);
    }
    else if (m_type == SAMPLE_BOOL_VEC4)
    {
        set4i(m_boolData[0], m_boolData[1], m_boolData[2], m_boolData[3]);
    }
}