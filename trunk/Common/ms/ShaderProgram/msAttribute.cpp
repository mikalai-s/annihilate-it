#include "msAttribute.h"
#include "msShaderProgram.h"
#include "../msCommon.h"

msAttribute::msAttribute()
{
    m_name = 0;
    m_location = -1;
}

msAttribute::msAttribute(string &name, GLint location)
{
    m_name = copyString(name);
    m_location = location;
}

msAttribute::~msAttribute(void)
{
    if(m_name != 0)
        delete m_name;
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
