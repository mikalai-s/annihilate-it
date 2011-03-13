/*
	MsGrid
*/

#ifndef _MsGrid_H
#define _MsGrid_H

#include "MsCommon.h"

typedef struct MsGrid
{
	int rowCount;
	int columnCount;

	generic *items; 
} MsGrid;


MsGrid* ms_grid_create_empty(int rowCount, int colCount);

void ms_grid_free(MsGrid **grid);

void ms_grid_set_item(MsGrid *grid, int y, int x, generic value);

generic ms_grid_get_item(MsGrid *grid, int y, int x);

void ms_grid_display(MsGrid *grid);

#endif

