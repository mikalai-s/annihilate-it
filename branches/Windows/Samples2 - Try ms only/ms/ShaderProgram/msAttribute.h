#pragma once

#include "GLES2/gl2.h"

class msShaderProgram;

class msAttribute
{

protected:
	const char *m_name;
	GLuint m_location;

	msShaderProgram *m_program;

		msAttribute();

public:

	msAttribute(const char *name, GLuint location);
	~msAttribute(void);

	virtual void setProgram(msShaderProgram *program);
};
