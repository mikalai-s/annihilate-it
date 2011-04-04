#pragma once

#include "GLES2/gl2.h"
#include "msAttribute.h"

class msShaderProgram;

class msTexture : public msAttribute
{
	GLuint m_unit;
	GLuint m_id;

public:
	msTexture(const char *name, GLuint unit, const char *fileName = 0);

	~msTexture(void);

	virtual void setProgram(msShaderProgram *program);

	virtual void link();

	GLuint getId();

	GLuint getUnit();
};
