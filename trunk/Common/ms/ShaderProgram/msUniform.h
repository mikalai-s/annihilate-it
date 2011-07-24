#pragma once

#include "../msGL.h"
#include "msAttribute.h"
#include <string>

using namespace std;

class msShaderProgram;

class msUniform : public msAttribute
{
    GLfloat m_floatData[4];
    GLboolean m_boolData[4];
    GLfloat m_matrixData[4][4];
    
    GLuint m_type;



public:
    msUniform(string &name);

    msUniform(string &name, GLfloat v);    

    msUniform(string &name, GLfloat v[], GLuint size = 1);

    msUniform(string &name, GLfloat matrix[4][4]);

    msUniform(string &name, GLboolean b);

    msUniform(string &name, GLboolean b[], GLuint size = 1);

    ~msUniform(void);

    virtual void setProgram(msShaderProgram *program);

    virtual void link();

    void set1f(GLfloat v0)
    {
        glUniform1f(m_location, v0);
    }

    void set2f(GLfloat v0, GLfloat v1)
    {
        glUniform2f(m_location, v0, v1);
    }

    void set3f(GLfloat v0, GLfloat v1, GLfloat v2)
    {
        glUniform3f(m_location, v0, v1, v2);
    }

    void set4f(GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        glUniform4f(m_location, v0, v1, v2, v3);
    }

    void set1i(GLint v0)
    {
        glUniform1i(m_location, v0);
    }

    void set2i(GLint v0, GLint v1)
    {
        glUniform2i(m_location, v0, v1);
    }

    void set3i(GLint v0, GLint v1, GLint v2)
    {
        glUniform3i(m_location, v0, v1, v2);
    }

    void set4i(GLint v0, GLint v1, GLint v2, GLint v3)
    {
        glUniform4i(m_location, v0, v1, v2, v3);
    }
    void set1fv(GLsizei count, const GLfloat *value)
    {
        glUniform1fv(m_location, count, value);
    }

    void set2fv(GLsizei count, const GLfloat *value)
    {
        glUniform2fv(m_location, count, value);
    }

    void set3fv(GLsizei count, const GLfloat *value)
    {
        glUniform3fv(m_location, count, value);
    }

    void set4fv(GLsizei count, const GLfloat *value)
    {
        glUniform4fv(m_location, count, value);
    }

    void set1iv(GLsizei count, const GLint *value)
    {
        glUniform1iv(m_location, count, value);
    }

    void set2iv(GLsizei count, const GLint *value)
    {
        glUniform2iv(m_location, count, value);
    }

    void set3iv(GLsizei count, const GLint *value)
    {
        glUniform3iv(m_location, count, value);
    }

    void set4iv(GLsizei count, const GLint *value)
    {
        glUniform4iv(m_location, count, value);
    }

    void setMatrix2fv(GLsizei count, GLboolean transpose, const GLfloat *value)
    {
        glUniformMatrix2fv(m_location, count, transpose, value);
    }

    void setMatrix3fv(GLsizei count, GLboolean transpose, const GLfloat *value)
    {
        glUniformMatrix3fv(m_location, count, transpose, value);
    }

    void setMatrix4fv(GLsizei count, GLboolean transpose, const GLfloat *value)
    {
        glUniformMatrix4fv(m_location, count, transpose, value);
    }
};
