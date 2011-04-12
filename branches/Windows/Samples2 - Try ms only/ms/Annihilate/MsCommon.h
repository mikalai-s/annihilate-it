#ifndef _MsCommon_H
#define _MsCommon_H

#include "../msGL.h"

typedef struct point
{
	float x;
	float y;
    float z;
} point;


typedef struct size
{
	float width;
	float height;
} size;

typedef struct color
{
    float r;
    float g;
    float b;
    float a;
} color;


typedef struct border
{
    int left;
    int top;
    int right;
    int bottom;
} border;


typedef union generic
{
    float f;
	unsigned int u;
	int i;
	char c[4];
    void *p;
} generic;


void ms_common_fill_color(color *color, float r, float g, float b, float a);

generic _ftog(float f);

generic _itog(int i);

generic _ptog(void *p);

generic ms_generic_empty();

#endif