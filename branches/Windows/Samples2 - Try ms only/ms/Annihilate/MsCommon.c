#include "MsCommon.h"




void ms_common_fill_color(color *color, float r, float g, float b, float a)
{
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}

generic _ftog(float f)	
{
	generic g;
	g.f = f;
	return g;
}


generic _itog(int i)
{
	generic g;
	g.i = i;
	return g;
}


generic _ptog(void *p)
{
	generic g;
	g.p = p;
	return g;
}


generic ms_generic_empty()
{
	generic g;
	g.i = 0;
	return g;
}