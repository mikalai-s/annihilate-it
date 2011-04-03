#include "msTexture.h"
#include "ImageObject.h"
#include "SimpleImageLoader.h"
#include "msShaderProgram.h"

msTexture::msTexture(const char *name, GLuint unit, const char *fileName)
{
	m_name = name;
	m_unit = unit;

	glGenTextures( 1, &m_id );
	glActiveTexture( GL_TEXTURE0 + m_unit );
	glBindTexture( GL_TEXTURE_2D, m_id );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	if(fileName != NULL)
	{
		SimpleImageLoader imgLoader;
		ImageObject *pImgObj;

		pImgObj = imgLoader.LoadImageFile( fileName );
		if ( pImgObj != NULL )
		{
			if ( pImgObj->GetNumChannels() == 3 )
			{
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, pImgObj->GetWidth(), pImgObj->GetHeight(),
					0, GL_RGB, GL_UNSIGNED_BYTE, pImgObj->GetDataPtr() );
			}
			else
			{
				//			assert( pImgObj->GetNumChannels() == 4 );
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pImgObj->GetWidth(), pImgObj->GetHeight(),
					0, GL_RGBA, GL_UNSIGNED_BYTE, pImgObj->GetDataPtr() );
			}
			delete pImgObj;
		}
	}	
}

msTexture::~msTexture(void)
{
}

void msTexture::setProgram(msShaderProgram *program)
{
	m_program = program;

	m_location = glGetUniformLocation( program->getHandle(), m_name);
}

GLuint msTexture::getId()
{
	return m_id;
}
