#include "msUniform.h"
#include "msShaderProgram.h"

#include "Uniform.h" // delete this

msUniform::msUniform(const char *name, GLfloat v)
{
    m_name = name;
	m_floatData[0] = v;	
	m_type = SAMPLE_FLOAT;
}

msUniform::msUniform(const char *name, GLfloat v[], GLuint size)
{
	m_name = name;

	for(GLuint i = 0; i < size; i ++)
		m_floatData[i] = v[i];

	m_type = (size == 2) ? SAMPLE_FLOAT_VEC2 :
			 (size == 3) ? SAMPLE_FLOAT_VEC3 :
			 (size == 4) ? SAMPLE_FLOAT_VEC4 : 0;
}

msUniform::msUniform(const char *name, GLfloat matrix[4][4])
{
	m_name = name;

	for(GLuint i = 0; i < 4; i ++)
		for(GLuint j = 0; j < 0; j ++)
			m_matrixData[i][j] = matrix[i][j];
	m_type = SAMPLE_FLOAT_MAT4;
}


msUniform::~msUniform(void)
{
}

void msUniform::setProgram(msShaderProgram *program)
{
	m_program = program;

	m_location = glGetUniformLocation( program->getHandle(), m_name);
}