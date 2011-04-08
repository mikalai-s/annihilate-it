#include "msTexture.h"
#include "../msImageObject.h"
#include "../msSimpleImageLoader.h"
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
		msSimpleImageLoader imgLoader;
		msImageObject *pImgObj;

		pImgObj = imgLoader.LoadImageFile( fileName );
		if ( pImgObj != NULL )
		{
			m_width = pImgObj->GetWidth();
			m_height = pImgObj->GetHeight();
			if ( pImgObj->GetNumChannels() == 3 )
			{
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height,
					0, GL_RGB, GL_UNSIGNED_BYTE, pImgObj->GetDataPtr() );
			}
			else
			{
				//			assert( pImgObj->GetNumChannels() == 4 );
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height,
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
}

void msTexture::link()
{
	m_location = glGetUniformLocation( m_program->getHandle(), m_name);
}

GLuint msTexture::getId()
{
	return m_id;
}

GLuint msTexture::getUnit()
{
	return m_unit;
}

GLsizei msTexture::getWidth()
{
	return m_width;
}

GLsizei msTexture::getHeight()
{
	return m_height;
}
