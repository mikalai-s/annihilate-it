/*
 *  MsPalette.c
 *  AnnihilateIt
 *
 *  Created by user on 11-03-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MsPalette.h"
#include "stdlib.h"


MsPalette* ms_palette_create()
{
    return (MsPalette*)calloc(1, sizeof(MsPalette));
}

void ms_palette_init(MsPalette* p, float colors[][4], int numColors)
{
    int i = 0;
    for(i = 0; i < numColors; i ++)
    {
        p->colors[i].r = colors[i][0];
        p->colors[i].g = colors[i][1];
        p->colors[i].b = colors[i][2];
        p->colors[i].a = colors[i][3];
    }
}

void ms_palette_free(MsPalette **p)
{
    free(*p);
    *p = 0;
}

void ms_palette_set_color(MsPalette *p, int index, color *color)
{
    if(index < MS_PALETTE_MAX_COLORS)
        p->colors[index] = *color;
}

color* ms_palette_get_color(MsPalette *p, int index)
{ 
    if(index < MS_PALETTE_MAX_COLORS)
        return &p->colors[index];
    return 0;
}

int ms_palette_get_free_index(MsPalette *p)
{
    int i;
    for(i = 0; i < MS_PALETTE_MAX_COLORS; i ++)
        if(p->colors[i].r == MS_PALETTE_EMPTY_COLOR_R &&
           p->colors[i].g == MS_PALETTE_EMPTY_COLOR_G &&
           p->colors[i].b == MS_PALETTE_EMPTY_COLOR_B &&
           p->colors[i].a == MS_PALETTE_EMPTY_COLOR_A)
            return i;
    return -1;
}

void ms_palette_set_free_index(MsPalette *p, int index)
{

    if(index < MS_PALETTE_MAX_COLORS)
        p->colors[index] = ms_palette_get_empty_color();
}


color ms_palette_get_empty_color()
{
    color empty;
    empty.r = MS_PALETTE_EMPTY_COLOR_R;
    empty.g = MS_PALETTE_EMPTY_COLOR_G;
    empty.b = MS_PALETTE_EMPTY_COLOR_B;
    empty.a = MS_PALETTE_EMPTY_COLOR_A;
    return empty;
}