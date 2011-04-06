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

	msFrameBuffer(GLuint id);

public:
	
	msFrameBuffer(const char *name, msTexture *texture);
	~msFrameBuffer(void);

	const char* getName();

	void bind();
	bool isComplete();
	msTexture* getTexture();

	static msFrameBuffer *main;
};