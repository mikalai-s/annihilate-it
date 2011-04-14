#include "MsCommon.h"



void ms_common_fill_color(msColor *color, float r, float g, float b, float a)
{
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}

msGeneric _ftog(float f)	
{
	msGeneric g;
	g.f = f;
	return g;
}


msGeneric _itog(int i)
{
	msGeneric g;
	g.i = i;
	return g;
}


msGeneric _ptog(void *p)
{
	msGeneric g;
	g.p = p;
	return g;
}


msGeneric ms_generic_empty()
{
	msGeneric g;
	g.i = 0;
	return g;
}