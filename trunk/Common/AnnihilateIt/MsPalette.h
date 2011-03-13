/*
 *  MsPalette.h
 *  AnnihilateIt
 *
 *  Created by user on 11-03-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _MsPalette_H
#define _MsPalette_H

#include "MsCommon.h"

#define MS_PALETTE_MAX_COLORS 100

#define MS_PALETTE_EMPTY_COLOR_R 0.0f
#define MS_PALETTE_EMPTY_COLOR_G 0.0f
#define MS_PALETTE_EMPTY_COLOR_B 0.0f
#define MS_PALETTE_EMPTY_COLOR_A 0.0f


typedef struct MsPalette
{
    color colors[MS_PALETTE_MAX_COLORS];
} MsPalette;



MsPalette* ms_palette_create();

void ms_palette_init(MsPalette* p, float[][4], int numColors);

void ms_palette_free(MsPalette **p);

void ms_palette_set_color(MsPalette *p, int index, color *color);

color* ms_palette_get_color(MsPalette *p, int index);

int ms_palette_get_free_index(MsPalette *p);

void ms_palette_set_free_index(MsPalette *p, int index);

color ms_palette_get_empty_color();


#endif


