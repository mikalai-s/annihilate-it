#include "msUniform.h"
#include "msShaderProgram.h"

#define SAMPLE_FLOAT 1
#define SAMPLE_FLOAT_VEC2 1
#define SAMPLE_FLOAT_VEC3 1
#define SAMPLE_FLOAT_VEC4 1
#define SAMPLE_FLOAT_MAT4 1

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
}

void msUniform::link()
{
	m_location = glGetUniformLocation( m_program->getHandle(), m_name);

	if(m_type == SAMPLE_FLOAT)
	{
		set1f(m_floatData[0]);
	}
	else if (m_type == SAMPLE_FLOAT_VEC2)
	{
		set2f(m_floatData[0], m_floatData[1]);
	}
	else if (m_type == SAMPLE_FLOAT_VEC3)
	{
		set3f(m_floatData[0], m_floatData[1], m_floatData[2]);
	}
	else if (m_type == SAMPLE_FLOAT_VEC4)
	{
		set4f(m_floatData[0], m_floatData[1], m_floatData[2], m_floatData[3]);
	}
	else if (m_type == SAMPLE_FLOAT_MAT4)
	{
		setMatrix4fv(1, GL_FALSE, m_floatData);
	}
}