/*
 *  MsPalette.c
 *  AnnihilateIt
 *
 *  Created by user on 11-03-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "msPalette.h"
#include "stdlib.h"


msPalette::msPalette(GLfloat colors[][4], GLuint numColors)
{
    for(GLuint i = 0; i < numColors; i ++)
    {
        this->colors[i].r = colors[i][0];
        this->colors[i].g = colors[i][1];
        this->colors[i].b = colors[i][2];
        this->colors[i].a = colors[i][3];
    }
}

msPalette::~msPalette()
{
}

void msPalette::setColor(GLuint index, msColor *color)
{
    if(index < MS_PALETTE_MAX_COLORS)
        this->colors[index] = *color;
}

msColor* msPalette::getColor(GLuint index)
{ 
    if(index < MS_PALETTE_MAX_COLORS)
        return &this->colors[index];
    return 0;
}

GLuint msPalette::getFreeIndex()
{
    for(GLuint i = 0; i < MS_PALETTE_MAX_COLORS; i ++)
        if(this->colors[i].r == MS_PALETTE_EMPTY_COLOR_R &&
           this->colors[i].g == MS_PALETTE_EMPTY_COLOR_G &&
           this->colors[i].b == MS_PALETTE_EMPTY_COLOR_B &&
           this->colors[i].a == MS_PALETTE_EMPTY_COLOR_A)
            return i;
    return -1;
}


void msPalette::setFreeIndex(GLuint index)
{
    if(index < MS_PALETTE_MAX_COLORS)
        this->colors[index] = getEmptyColor();
}


msColor msPalette::getEmptyColor()
{
    msColor empty;
    empty.r = MS_PALETTE_EMPTY_COLOR_R;
    empty.g = MS_PALETTE_EMPTY_COLOR_G;
    empty.b = MS_PALETTE_EMPTY_COLOR_B;
    empty.a = MS_PALETTE_EMPTY_COLOR_A;
    return empty;
}