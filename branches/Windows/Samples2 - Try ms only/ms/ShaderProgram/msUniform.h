#pragma once

#include "GLES2/gl2.h"
#include "msAttribute.h"

class msShaderProgram;

class msUniform : public msAttribute
{
	GLfloat m_floatData[4];
	GLfloat m_matrixData[4][4];
	GLuint m_type;

public:
	msUniform(const char *name, GLfloat v);	

	msUniform(const char *name, GLfloat v[], GLuint size = 1)	;

	msUniform(const char *name, GLfloat matrix[4][4]);

	~msUniform(void);

	virtual void setProgram(msShaderProgram *program);
};
