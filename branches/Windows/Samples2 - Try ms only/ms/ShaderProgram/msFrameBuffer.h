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

	const char *m_name;
	msTexture *m_texture;
	GLuint m_id;

	GLsizei m_width;
	GLsizei m_height;

	GLboolean m_requiresBufferDeletion;

	msFrameBuffer(GLuint id);

public:
	
	msFrameBuffer(string &name, msTexture *texture);
	~msFrameBuffer(void);

	const char* getName();

	void bind();
	bool isComplete();
	msTexture* getTexture();
	void setSize( GLint width, GLint height );
	static msFrameBuffer *main;
};