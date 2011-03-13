#ifndef _MsBoxGrid_H
#define _MsBoxGrid_H

#include "MsBox.h"
#include "MsGrid.h"
#include "MsPalette.h"


#define MS_BOX_SHIFT_DOWN 0
#define MS_BOX_SHIFT_LEFT 1
#define MS_BOX_SHIFT_TOP 2
#define MS_BOX_SHIFT_RIGHT 3


typedef struct MsBoxGrid
{
	size size;

	MsGrid *grid;
    
    MsPalette *palette;
} MsBoxGrid;



MsBoxGrid *ms_boxgrid_create_from_pattern(MsPalette *palette, int *pattern, int numRows, int numCols, float screenHeight, float screenWidth);

MsBoxGrid *ms_boxgrid_create_from_random_pattern(MsPalette *palette, int numColors, int numRows, int numCols, float gridHeight, float gridWidth);

void ms_boxgrid_display(MsBoxGrid *grid);

void ms_boxgrid_free(MsBoxGrid **grid);

void ms_boxgrid_remove_similar_items(MsBoxGrid *grid, int y, int x);

void ms_boxgrid_shift_pendent_boxes(MsBoxGrid *grid, int direction);

int ms_boxgrid_check_neighbours(MsBoxGrid *g, int y1, int x1, int y2, int x2);

void ms_boxgrid_unit_test();


#endif