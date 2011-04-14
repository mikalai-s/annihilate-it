#pragma once

#include "../msGL.h"

struct msPoint
{
	GLfloat x;
	GLfloat y;
    GLfloat z;
};

struct msSize
{
	GLfloat width;
	GLfloat height;
};

struct msColor
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};


struct msBorder
{
    GLint left;
    GLint top;
    GLint right;
    GLint bottom;
};


union msGeneric
{
    GLfloat f;
	GLuint u;
	GLint i;
	char c[4];
    void *p;
};



void ms_common_fill_color(msColor *c, float r, float g, float b, float a);


msGeneric _ftog(float f);



msGeneric _itog(int i);



msGeneric _ptog(void *p);



msGeneric ms_generic_empty();
