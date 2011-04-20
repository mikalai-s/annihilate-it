#pragma once

#include "../msGL.h"
#include "msAttribute.h"
#include <string>
#include "../msCommon.h"

using namespace std;

class msShaderProgram;

class msTexture : public msAttribute
{
	GLuint m_unit;
	GLuint m_id;
	GLsizei m_width;
	GLsizei m_height;
public:
	msTexture(string &name, GLuint unit, const char *fileName = 0);

	~msTexture(void);

	virtual void setProgram(msShaderProgram *program);

	virtual void link();

	GLuint getId();

	GLuint getUnit();
	GLsizei getWidth();
	GLsizei getHeight();
};