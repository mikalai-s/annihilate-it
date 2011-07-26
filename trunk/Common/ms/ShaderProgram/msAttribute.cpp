#include "msAttribute.h"
#include "msShaderProgram.h"
#include "../msCommon.h"

msAttribute::msAttribute()
{
    this->name = 0;
    this->location = -1;
}

msAttribute::msAttribute(string &name, GLint location)
{
    this->name = copyString(name);
    this->location = location;
}

msAttribute::~msAttribute(void)
{
    if(this->name != 0)
        delete this->name;
}

void msAttribute::setProgram(msShaderProgram *program)
{
    this->program = program;

    glBindAttribLocation(program->getHandle(), this->location, this->name);
}

const char* msAttribute::getName()
{
    return this->name;
}

void msAttribute::link()
{
}

const GLint msAttribute::getLocation()
{
    return this->location;
}
