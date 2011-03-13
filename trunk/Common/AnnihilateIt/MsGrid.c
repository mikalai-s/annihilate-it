#include "MsGrid.h"
#include "stdlib.h"
#include "stdio.h"


MsGrid* ms_grid_create_empty(int rowCount, int colCount)
{
	MsGrid *grid = (MsGrid*)malloc(sizeof(MsGrid));
	grid->rowCount = rowCount;
	grid->columnCount = colCount;
	grid->items = (generic*)calloc(rowCount * colCount, sizeof(generic));

	return grid;
}

void ms_grid_free(MsGrid **grid)
{
	free((*grid)->items);
	free((*grid));
	*grid = 0;
}

int _ms_grid_get_index_by_yx(MsGrid *grid, int y, int x)
{
	return y * grid->columnCount + x;
}

void ms_grid_set_item(MsGrid *grid, int y, int x, generic value)
{
	if(y < grid->rowCount && x < grid->columnCount)
		grid->items[_ms_grid_get_index_by_yx(grid, y, x)] = value;
}

generic ms_grid_get_item(MsGrid *grid, int y, int x)
{
	if(y >= 0 && y < grid->rowCount && x >= 0 && x < grid->columnCount)
		return grid->items[_ms_grid_get_index_by_yx(grid, y, x)];
	return ms_generic_empty();
}


void ms_grid_display(MsGrid *grid)
{
	int y, x;
	for(y = 0; y < grid->rowCount; y ++)
	{
		for(x = 0; x < grid->columnCount; x ++)
			printf("%d ", grid->items[_ms_grid_get_index_by_yx(grid, y, x)].i);
		printf("\r\n");
	}		
}

void ms_grid_unit_test()
{
}