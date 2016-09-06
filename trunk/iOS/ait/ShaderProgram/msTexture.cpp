#include "msTexture.h"
#include "../msImageObject.h"
#include "../msSimpleImageLoader.h"
#include "msShaderProgram.h"


msTexture::msTexture(string &name, GLuint unit, string &fileName)
{
    init(name, unit, fileName.c_str());
}

msTexture::msTexture(string &name, GLuint unit)
{
    init(name, unit);
}

void msTexture::init(string &name, GLuint unit, const char *fileName)
{
    this->name = copyString(name);
    this->unit = unit;

    glGenTextures( 1, &this->id );
    active();
    bind();
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    if(fileName != 0)
    {
        msSimpleImageLoader imgLoader;
        msImageObject *pImgObj;

        pImgObj = imgLoader.LoadImageFile( fileName );
        if ( pImgObj != NULL )
        {
            this->width = pImgObj->GetWidth();
            this->height = pImgObj->GetHeight();
            if ( pImgObj->GetNumChannels() == 3 )
            {
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImgObj->GetDataPtr() );
            }
            else
            {
                //            assert( pImgObj->GetNumChannels() == 4 );
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImgObj->GetDataPtr() );
            }
            delete pImgObj;
        }
    }    
}

msTexture::~msTexture(void)
{
    glDeleteTextures(1, &this->id);
}

void msTexture::setProgram(msShaderProgram *program)
{
    this->program = program;
}

void msTexture::link()
{
    this->location = glGetUniformLocation( this->program->getHandle(), this->name);
}

GLuint msTexture::getId()
{
    return this->id;
}

GLuint msTexture::getUnit()
{
    return this->unit;
}

GLsizei msTexture::getWidth()
{
    return this->width;
}

GLsizei msTexture::getHeight()
{
    return this->height;
}

void msTexture::bind()
{
    glBindTexture(GL_TEXTURE_2D, getId());
}

void msTexture::active()
{
    glActiveTexture(GL_TEXTURE0 + getUnit());
}
