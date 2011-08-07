#pragma once

#include "../msGL.h"
#include "msAttribute.h"
#include <string>
#include "../msCommon.h"

using namespace std;

class msShaderProgram;

class msTexture : public msAttribute
{
    friend class msFrameBuffer;
    
    GLuint unit;
    GLuint id;
    GLsizei width;
    GLsizei height;

    void init(string &name, GLuint unit, const char *fileName = 0);

public:
    msTexture(string &name, GLuint unit, string &fileName);

    msTexture(string &name, GLuint unit);

    ~msTexture(void);

    virtual void setProgram(msShaderProgram *program);

    virtual void link();

    GLuint getId();

    GLuint getUnit();
    GLsizei getWidth();
    GLsizei getHeight();

    void bind();
    void active();
};
