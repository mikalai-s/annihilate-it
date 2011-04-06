#include "msAttribute.h"
#include "msShaderProgram.h"

msAttribute::msAttribute()
{
	m_name = "";
	m_location = -1;
}

msAttribute::msAttribute(const char *name, GLint location)
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

const char* msAttribute::getName()
{
	return m_name;
}

void msAttribute::link()
{
}

const GLint msAttribute::getLocation()
{
	return m_location;
}
