#pragma once

#include "../msGL.h"

class msShaderProgram;

class msAttribute
{

protected:
	const char *m_name;
	GLint m_location;

	msShaderProgram *m_program;

	msAttribute();

public:

	msAttribute(const char *name, GLint location);
	~msAttribute(void);

	virtual void setProgram(msShaderProgram *program);

	// is called when program is getting linked
	virtual void link();

	const char *getName();

	const GLint getLocation();

	void enableVertexArray()
	{
		glEnableVertexAttribArray(m_location);
	}

	void disableVertexArray()
	{
		glDisableVertexAttribArray(m_location);
	}

	void setPointer(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
	{
		glVertexAttribPointer(m_location, size, type, normalized, stride, pointer);
	}

	void setPointerAndEnable(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
	{
		setPointer(size, type, normalized, stride, pointer);
		enableVertexArray();
	}

	void set1f(GLfloat v0)
	{
		glVertexAttrib1f(m_location, v0);
	}

	void set2f(GLfloat v0, GLfloat v1)
	{
		glVertexAttrib2f(m_location, v0, v1);
	}

	void set3f(GLfloat v0, GLfloat v1, GLfloat v2)
	{
		glVertexAttrib3f(m_location, v0, v1, v2);
	}

	void set4f(GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		glVertexAttrib4f(m_location, v0, v1, v2, v3);
	}

	void set1fv(const GLfloat *v)
	{
		glVertexAttrib1fv(m_location, v);
	}

	void set2fv(const GLfloat *v)
	{
		glVertexAttrib2fv(m_location, v);
	}

	void set3fv(const GLfloat *v)
	{
		glVertexAttrib3fv(m_location, v);
	}

	void set4fv(const GLfloat *v)
	{
		glVertexAttrib4fv(m_location, v);
	}
};
