#pragma once

#include "../msGL.h"
#include "msTexture.h"
#include <string>

using namespace std;

class msShaderPrograms;

class msFrameBuffer
{
    friend class msShaderPrograms;

private:

    const char *name;
    msTexture *texture;
    GLuint id;

    GLsizei width;
    GLsizei height;

    GLboolean requiresBufferDeletion;

    msFrameBuffer(GLuint id);

public:
    
    msFrameBuffer(string &name, GLuint textureUnit);
    ~msFrameBuffer(void);

    const char* getName();

    void bind();
    bool isComplete();
    msTexture* getTexture();
    void setSize( GLint width, GLint height );
    static msFrameBuffer *main;
};