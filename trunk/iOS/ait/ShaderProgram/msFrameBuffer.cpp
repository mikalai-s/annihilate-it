#include "msFrameBuffer.h"

// The constructor is used to wrap existing FBO
msFrameBuffer::msFrameBuffer(GLuint id)
{
    this->name = 0;
    this->id = id;
    this->texture = 0;

    this->requiresBufferDeletion = false;
}

// The constructor is used to create new FBOB
msFrameBuffer::msFrameBuffer(string &name, GLuint textureUnit)
{
    this->name = copyString(name);
    this->texture = new msTexture(name, textureUnit);

    // Generate handles for two Frame Buffer Objects
    glGenFramebuffers(1, &this->id);

    this->requiresBufferDeletion = true;
}

msFrameBuffer::~msFrameBuffer(void)
{
    if(this->name != 0)
        delete this->name;

    if(this->texture != 0)
        delete this->texture;

    if(this->requiresBufferDeletion)
        glDeleteFramebuffers(1, &this->id);
}

const char* msFrameBuffer::getName()
{
    return this->name;
}

void msFrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

bool msFrameBuffer::isComplete()
{
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

msTexture* msFrameBuffer::getTexture()
{
    return this->texture;
}

void msFrameBuffer::setSize( GLint width, GLint height )
{
    this->width = width;
    this->height = height;
    
    // init framebuffer and texture to render to (for fire)    
    this->texture->active();
    
    // Bind and configure the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);    
    
    // Attach the texture to the first color buffer of an FBO and clear it
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture->getId(), 0);
}
