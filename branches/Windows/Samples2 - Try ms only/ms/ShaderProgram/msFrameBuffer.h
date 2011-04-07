#pragma once

#include "GLES2/gl2.h"
#include "msTexture.h"

class msShaderPrograms;

class msFrameBuffer
{
	friend msShaderPrograms;

private:

	const char *m_name;
	msTexture *m_texture;
	GLuint m_id;

	GLsizei m_width;
	GLsizei m_height;

	msFrameBuffer(GLuint id);

public:
	
	msFrameBuffer(const char *name, msTexture *texture);
	~msFrameBuffer(void);

	const char* getName();

	void bind();
	bool isComplete();
	msTexture* getTexture();
	void setSize( GLint width, GLint height );
	static msFrameBuffer *main;
};