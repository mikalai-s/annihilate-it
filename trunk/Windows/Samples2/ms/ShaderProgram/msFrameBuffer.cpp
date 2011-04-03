#include "msFrameBuffer.h"

msFrameBuffer::msFrameBuffer(const char *name, msTexture *texture)
{
	m_name = name;
	m_texture = texture;
	// init framebuffer and texture to render to (for fire)	

	// Bind and configure the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1000, 1000, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);	

	// Generate handles for two Frame Buffer Objects
	glGenFramebuffers(1, &m_id);

	// Attach the texture to the first color buffer of an FBO and clear it
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getId(), 0);
}

msFrameBuffer::~msFrameBuffer(void)
{
		// don't forget release them:
	
	// Delete the FBOs
	//glDeleteFramebuffers(2, m_id);

	
	
}
