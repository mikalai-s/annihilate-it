#include "msAttribute.h"
#include "msShaderProgram.h"

msAttribute::msAttribute()
{
	m_name = "";
	m_location = -1;
}

msAttribute::msAttribute(const char *name, GLuint location)
{
	m_name = name;
	m_location = location;
}

msAttribute::~msAttribute(void)
{
}

void msAttribute::setProgram(msShaderProgram *program)
{
	m_program = program;

	glBindAttribLocation(program->getHandle(), m_location, m_name);
}