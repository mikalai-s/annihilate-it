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
    this->name = copyString(name);
    this->type = -1;
}

msUniform::msUniform(string &name, GLfloat v)
{
    this->name = copyString(name);

    this->floatData[0] = v;
    this->type = SAMPLE_FLOAT;
}

msUniform::msUniform(string &name, GLfloat v[], GLuint size)
{
    this->name = copyString(name);

    for(GLuint i = 0; i < size; i ++)
        this->floatData[i] = v[i];

    this->type = (size == 2) ? SAMPLE_FLOAT_VEC2 :
             (size == 3) ? SAMPLE_FLOAT_VEC3 :
             (size == 4) ? SAMPLE_FLOAT_VEC4 : 0;
}

msUniform::msUniform(string &name, GLfloat matrix[4][4])
{
    this->name = copyString(name);

    for(GLuint i = 0; i < 4; i ++)
        for(GLuint j = 0; j < 0; j ++)
            this->matrixData[i][j] = matrix[i][j];
    this->type = SAMPLE_FLOAT_MAT4;
}

msUniform::msUniform(string &name, GLboolean b)
{
    this->name = copyString(name);

    this->floatData[0] = b;
    this->type = SAMPLE_BOOL;
}

msUniform::msUniform(string &name, GLboolean b[], GLuint size)
{
    this->name = copyString(name);

    for(GLuint i = 0; i < size; i ++)
        this->boolData[i] = b[i];

    this->type = (size == 2) ? SAMPLE_BOOL_VEC2 :
             (size == 3) ? SAMPLE_BOOL_VEC3 :
             (size == 4) ? SAMPLE_BOOL_VEC4 : 0;
}


msUniform::~msUniform(void)
{
}

void msUniform::setProgram(msShaderProgram *program)
{
    this->program = program;
}

void msUniform::link()
{
    this->location = glGetUniformLocation( this->program->getHandle(), this->name);

    if(this->type == SAMPLE_FLOAT)
    {
        set1f(this->floatData[0]);
    }
    else if (this->type == SAMPLE_FLOAT_VEC2)
    {
        set2f(this->floatData[0], this->floatData[1]);
    }
    else if (this->type == SAMPLE_FLOAT_VEC3)
    {
        set3f(this->floatData[0], this->floatData[1], this->floatData[2]);
    }
    else if (this->type == SAMPLE_FLOAT_VEC4)
    {
        set4f(this->floatData[0], this->floatData[1], this->floatData[2], this->floatData[3]);
    }
    else if (this->type == SAMPLE_FLOAT_MAT4)
    {
        setMatrix4fv(1, GL_FALSE, this->floatData);
    }
    else if(this->type == SAMPLE_BOOL)
    {
        set1i(this->boolData[0]);
    }
    else if (this->type == SAMPLE_BOOL_VEC2)
    {
        set2i(this->boolData[0], this->boolData[1]);
    }
    else if (this->type == SAMPLE_BOOL_VEC3)
    {
        set3i(this->boolData[0], this->boolData[1], this->boolData[2]);
    }
    else if (this->type == SAMPLE_BOOL_VEC4)
    {
        set4i(this->boolData[0], this->boolData[1], this->boolData[2], this->boolData[3]);
    }
}