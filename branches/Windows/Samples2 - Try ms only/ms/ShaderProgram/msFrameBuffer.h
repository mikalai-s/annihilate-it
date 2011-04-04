#pragma once

#include "GLES2/gl2.h"
#include "msTexture.h"

class msFrameBuffer
{
	const char *m_name;
	msTexture *m_texture;
	GLuint m_id;

public:
	msFrameBuffer(const char *name, msTexture *texture);
	~msFrameBuffer(void);

	const char* getName();
};
