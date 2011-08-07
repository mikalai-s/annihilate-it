#pragma once

#include "../msGL.h"
#include <string>

using namespace std;

class msShaderProgram;

class msAttribute
{

protected:
    const char *name;
    GLint location;

    msShaderProgram *program;

    msAttribute();

public:

    msAttribute(string &name, GLint location);
    ~msAttribute(void);

    virtual void setProgram(msShaderProgram *program);

    // is called when program is getting linked
    virtual void link();

    const char *getName();

    const GLint getLocation();

    void enableVertexArray()
    {
        glEnableVertexAttribArray(this->location);
    }

    void disableVertexArray()
    {
        glDisableVertexAttribArray(this->location);
    }

    void setPointer(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
    {
        glVertexAttribPointer(this->location, size, type, normalized, stride, pointer);
    }

    void setPointerAndEnable(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
    {
        setPointer(size, type, normalized, stride, pointer);
        enableVertexArray();
    }

    void set1f(GLfloat v0)
    {
        glVertexAttrib1f(this->location, v0);
    }

    void set2f(GLfloat v0, GLfloat v1)
    {
        glVertexAttrib2f(this->location, v0, v1);
    }

    void set3f(GLfloat v0, GLfloat v1, GLfloat v2)
    {
        glVertexAttrib3f(this->location, v0, v1, v2);
    }

    void set4f(GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        glVertexAttrib4f(this->location, v0, v1, v2, v3);
    }

    void set1fv(const GLfloat *v)
    {
        glVertexAttrib1fv(this->location, v);
    }

    void set2fv(const GLfloat *v)
    {
        glVertexAttrib2fv(this->location, v);
    }

    void set3fv(const GLfloat *v)
    {
        glVertexAttrib3fv(this->location, v);
    }

    void set4fv(const GLfloat *v)
    {
        glVertexAttrib4fv(this->location, v);
    }
};
