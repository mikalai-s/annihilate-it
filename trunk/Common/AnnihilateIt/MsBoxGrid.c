#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#include "MsBoxGrid.h"
#include "MsGrid.h"
#include "MsList.h"


void _ms_boxgrid_refresh_borders(MsBoxGrid *boxGrid)
{
    int y, x;
    for(y = 0; y < boxGrid->grid->rowCount; y ++)
	{
		for(x = 0; x < boxGrid->grid->columnCount; x ++)
		{
			MsBox *box = (MsBox*)ms_grid_get_item(boxGrid->grid, y, x).p;
            
            box->border.left = !ms_boxgrid_check_neighbours(boxGrid, y, x-1, y, x);
            box->border.top = !ms_boxgrid_check_neighbours(boxGrid, y-1, x, y, x);
            box->border.right = !ms_boxgrid_check_neighbours(boxGrid, y, x, y, x+1);
            box->border.bottom = !ms_boxgrid_check_neighbours(boxGrid, y, x, y+1, x);
		}
	}
}


MsBoxGrid *ms_boxgrid_create_from_pattern(MsPalette *palette, int *pattern, int numRows, int numCols, float gridHeight, float gridWidth)
{
	int y, x;

	float width = gridWidth / numCols, height = gridHeight / numRows;
	float curx = 0, cury = 0;
    
	MsBoxGrid *boxGrid = (MsBoxGrid*)malloc(sizeof(MsBoxGrid));
	boxGrid->grid = ms_grid_create_empty(numRows, numCols);
    boxGrid->palette = palette;

	boxGrid->size.width = gridWidth;
	boxGrid->size.height = gridHeight;

	for(y = 0; y < numRows; y ++)
	{
		curx = 0;
		for(x = 0; x < numCols; x ++)
		{
			MsBox *box = ms_box_create(curx, cury, width, height, pattern[y * numCols + x]);
			ms_grid_set_item(boxGrid->grid, y, x, _ptog(box));

			curx += width;
		}

		cury += height;
	}
    
    _ms_boxgrid_refresh_borders(boxGrid);

	return boxGrid;
}


int _ms_boxgrid_get_index_by_yx(int y, int x, int numCols)
{
	return y * numCols + x;
}

int* _ms_boxgrid_generate_random_pattern(int numRows, int numCols, int numColors)
{
	int *pattern = (int*)malloc(sizeof(int) * numRows * numCols);
	int y, x;
    
	srand(time(0));
    
	for(y = 0; y < numRows; y ++)
		for(x = 0; x < numCols; x ++)
			pattern[_ms_boxgrid_get_index_by_yx(y, x, numCols)] = rand() % numColors + 1;
    
	return pattern;
}


MsBoxGrid *ms_boxgrid_create_from_random_pattern(MsPalette *palette, int numColors, int numRows, int numCols, float gridHeight, float gridWidth)
{
    int *pattern = _ms_boxgrid_generate_random_pattern(numRows, numCols, numColors);
    
    return ms_boxgrid_create_from_pattern(palette, pattern, numRows, numCols, gridHeight, gridWidth);
}



void ms_boxgrid_free(MsBoxGrid **g)
{
	MsBoxGrid *grid = *g;
	int x, y;
	for(y = 0; y < grid->grid->rowCount; y ++)
	{
		for(x = 0; x < grid->grid->columnCount; x ++)
		{
			MsBox *item = (MsBox*)ms_grid_get_item(grid->grid, y, x).p;
			ms_box_free(&item);
		}
	}

	ms_grid_free(&grid->grid);
	free(grid);
	*g = 0;
}



void ms_boxgrid_display(MsBoxGrid *grid)
{
	int y, x;
	for(y = 0; y < grid->grid->rowCount; y ++)
	{
		for(x = 0; x < grid->grid->columnCount; x ++)
		{
			MsBox *box = (MsBox*)ms_grid_get_item(grid->grid, y, x).p;
			if(ms_box_is_visible(box))
				printf("%d:(%3.0f,%3.0f)  ", box->colorIndex, box->location.y, box->location.x);
			else
				printf("      -      ");
		}
		printf("\r\n");
	}
	printf("\r\n");
}

void ms_boxgrid_display2(MsBoxGrid *grid)
{
	int y, x;
	for(y = 0; y < grid->grid->rowCount; y ++)
	{
		for(x = 0; x < grid->grid->columnCount; x ++)
		{
			MsBox *box = (MsBox*)ms_grid_get_item(grid->grid, y, x).p;
			if(ms_box_is_visible(box))
				printf("%d", box->colorIndex);
			else
				printf(" ");
		}
		printf("\r\n");
	}
	printf("\r\n");
}


void _hiding1(MsAnimation *anim)
{
    MsBox *box = (MsBox*)anim->from.p;
    
    box->location.z = 1.0f;
    
    anim->rAngle += 1.0f;
    anim->rVector.x = 1.0f;
    anim->rVector.y = 0.0f;
    anim->rVector.z = 0.0f;
}




void _ms_boxgrid_animate_box_hiding(MsBoxGrid *grid, MsList *boxes)
{    
    //ms_box_copy(box, box->boxToAnimate);
    
    // add animation
   // MsAnimation *sizeAnimation = ms_animation_create(_ptog(box->boxToAnimate), _itog(100), 0, 100, _hiding1);
    //ms_list_add_item(box->animations->list, _ptog(sizeAnimation));    
    
    //sizeAnimation = ms_animation_create(_ptog(&box->boxToAnimate->location), _itog(100), 10, 20, _hiding2);
    // ms_list_add_item(box->animations->list, _ptog(sizeAnimation));
}



void _removeSimilarBoxes(MsBoxGrid *grid, int y, int x, int c, MsList *removedBoxes)
{
	MsBox *box;

    // out of range
    if(x < 0 || y < 0 || x >= grid->grid->columnCount || y >= grid->grid->rowCount)
        return;
    
    box = (MsBox*)ms_grid_get_item(grid->grid, y, x).p;
	if(!ms_box_is_visible(box))
		return;
    
    // if the neighbour has different colour
    if(box->colorIndex != c)
        return;
    
	// make the box invisible
	ms_box_make_invisible(box);
    
    // add it to list
    ms_list_add_item(removedBoxes, _ptog(box));
       
    _removeSimilarBoxes(grid, y, x-1, c, removedBoxes);
    _removeSimilarBoxes(grid, y-1, x, c, removedBoxes);
    _removeSimilarBoxes(grid, y, x+1, c, removedBoxes);
    _removeSimilarBoxes(grid, y+1, x, c, removedBoxes);
}


int _ms_boxgrid_has_similar_neighbour(MsBoxGrid *grid, int y, int x, int colorIndex)
{
    MsBox *neighbour;
    
    if(x < 0 || x >= grid->grid->columnCount || y < 0 || y >= grid->grid->rowCount)
        return 0;
    
    if(x-1 >= 0)
    {
        neighbour = (MsBox*)ms_grid_get_item(grid->grid, y, x-1).p;
        if(neighbour->colorIndex == colorIndex)
            return 1;
    }
    if(x+1 < grid->grid->columnCount)
    {
        neighbour = (MsBox*)ms_grid_get_item(grid->grid, y, x+1).p;
        if(neighbour->colorIndex == colorIndex)
            return 1;
    }
    if(y-1 >= 0)
    {
        neighbour = (MsBox*)ms_grid_get_item(grid->grid, y-1, x).p;
        if(neighbour->colorIndex == colorIndex)
            return 1;
    }
    if(y+1 < grid->grid->rowCount)
    {
        neighbour = (MsBox*)ms_grid_get_item(grid->grid, y+1, x).p;
        if(neighbour->colorIndex == colorIndex)
            return 1;
    }
    return 0;    
}


void ms_boxgrid_remove_similar_items(MsBoxGrid *grid, int y, int x)
{
    MsList *removedBoxes = ms_list_create_empty();
	MsBox *box = (MsBox*)ms_grid_get_item(grid->grid, y, x).p;
	if(ms_box_is_visible(box))
    {
        if(_ms_boxgrid_has_similar_neighbour(grid, y, x, box->colorIndex))
        {           
            _removeSimilarBoxes(grid, y, x, box->colorIndex, removedBoxes);
           
            _ms_boxgrid_animate_box_hiding(grid, removedBoxes);
        }
    }
    ms_list_free(&removedBoxes);
}


void _exchangeBoxes(MsGrid *grid, int y1, int x1, int y2, int x2)
{
	MsBox *box1 = (MsBox*)ms_grid_get_item(grid, y1, x1).p;
	MsBox *box2 = (MsBox*)ms_grid_get_item(grid, y2, x2).p;

	ms_grid_set_item(grid, y1, x1, _ptog(box2));
	ms_grid_set_item(grid, y2, x2, _ptog(box1));
}

void _linearFalling(MsAnimation *animation)
{
	point *from = (point*)(animation->from.p);
	point *to = (point*)(animation->to.p);
	
	from->x += (to->x - from->x) / animation->count;
	from->y += (to->y - from->y) / animation->count;
}

void _linearFalling2(MsAnimation *animation)
{
	point *from = (point*)(animation->from.p);
    switch(animation->to.i)
    {
        case MS_BOX_SHIFT_TOP:
            from->y += 0.2f;
            break;
            
        case MS_BOX_SHIFT_LEFT:
            from->x += 0.2f;
            break;
            
        case MS_BOX_SHIFT_RIGHT:
            from->x -= 0.2f;
            break;
            
        default:
            from->y -= 0.2f;
    }
    
}


void _exchangeBoxesWithAnimation(MsGrid *grid, int y1, int x1, int y2, int x2, int direction)
{
	MsAnimation *animation;
	point tempLocation;
	int times = 10;	

	MsBox *box1 = (MsBox*)ms_grid_get_item(grid, y1, x1).p;
	MsBox *box2 = (MsBox*)ms_grid_get_item(grid, y2, x2).p;
    
	ms_grid_set_item(grid, y1, x1, _ptog(box2));
	ms_grid_set_item(grid, y2, x2, _ptog(box1));
    
    tempLocation = box1->location;
    box1->location = box2->location;
    box2->location = tempLocation;
    
    box1->boxToAnimate->border = box1->border;
  
	animation = ms_animation_create(_ptog(&box1->boxToAnimate->location), _ptog(&box1->location), 0, times, _linearFalling);
	ms_list_add_item(box1->animations->list, _ptog(animation));
    
    animation = ms_animation_create(_ptog(&box1->boxToAnimate->location), _itog(direction), times, 10, _linearFalling2);
	ms_list_add_item(box1->animations->list, _ptog(animation));
    
    animation = ms_animation_create(_ptog(&box1->boxToAnimate->location), _ptog(&box1->location), times + 10, 1, _linearFalling);
	ms_list_add_item(box1->animations->list, _ptog(animation));
}

void _shiftDown(MsBoxGrid *grid)
{
	int y, x, yy;
	for(x = 0; x < grid->grid->columnCount; x ++)
	{
		y = grid->grid->rowCount - 1;
		while(y >= -1)
		{
			if(!ms_box_is_visible((MsBox*)ms_grid_get_item(grid->grid, y, x).p))
			{
				// y < 0 when there are no empty boxes
				if(y < 0)
					break;

				yy = y - 1; // note: dependent on direction
				while(yy >= -1 && !ms_box_is_visible((MsBox*)ms_grid_get_item(grid->grid, yy, x).p))
					yy --;

				// yy < -1 when there are no items to shift
				if(yy <= -1)
					break;
                
				// shift found box
				_exchangeBoxesWithAnimation(grid->grid, yy, x, y, x, MS_BOX_SHIFT_DOWN);
			}
			
			y --;
		}
	}
}


void _shiftTop(MsBoxGrid *grid)
{
	int y, x, yy;
    
	for(x = 0; x < grid->grid->columnCount; x ++)
	{
		y = 0;
		while(y < grid->grid->rowCount)
		{
			if(!ms_box_is_visible((MsBox*)ms_grid_get_item(grid->grid, y, x).p))
			{
				// y < 0 when there are no empty boxes
				if(y >= grid->grid->rowCount)
					break;

				yy = y + 1; // note: dependent on direction
				while(yy <= grid->grid->rowCount && !ms_box_is_visible((MsBox*)ms_grid_get_item(grid->grid, yy, x).p))
					yy ++;

				// yy < -1 when there are no items to shift
				if(yy > grid->grid->rowCount - 1)
					break;

				// shift found box
				_exchangeBoxesWithAnimation(grid->grid, yy, x, y, x, MS_BOX_SHIFT_TOP);
			}
			
			y ++;
		}
	}
}


void _shiftRight(MsBoxGrid *grid)
{
	int y, x, xx;
	for(y = 0; y < grid->grid->rowCount; y ++)
	{
		x = grid->grid->columnCount - 1;
		while(x >= -1)
		{
			if(!ms_box_is_visible((MsBox*)ms_grid_get_item(grid->grid, y, x).p))
			{
				// y < 0 when there are no empty boxes
				if(x < 0)
					break;

				xx = x - 1; // note: dependent on direction
				while(xx >= -1 && !ms_box_is_visible((MsBox*)ms_grid_get_item(grid->grid, y, xx).p))
					xx --;

				// yy < -1 when there are no items to shift
				if(xx <= -1)
					break;

				// shift found box
				_exchangeBoxesWithAnimation(grid->grid, y, xx, y, x, MS_BOX_SHIFT_RIGHT);
			}
			
			x --;
		}
	}
}


void _shiftLeft(MsBoxGrid *grid)
{
	int y, x, xx;
	for(y = 0; y < grid->grid->rowCount; y ++)
	{
		x = 0;
		while(x < grid->grid->columnCount)
		{
			if(!ms_box_is_visible((MsBox*)ms_grid_get_item(grid->grid, y, x).p))
			{
				// y < 0 when there are no empty boxes
				if(x >= grid->grid->columnCount)
					break;

				xx = x + 1; // note: dependent on direction
				while(xx <= grid->grid->columnCount && !ms_box_is_visible((MsBox*)ms_grid_get_item(grid->grid, y, xx).p))
					xx ++;

				// yy < -1 when there are no items to shift
				if(xx > grid->grid->columnCount - 1)
					break;

				// shift found box
				_exchangeBoxesWithAnimation(grid->grid, y, xx, y, x, MS_BOX_SHIFT_LEFT);
			}
			
			x ++;
		}
	}
}


void ms_boxgrid_shift_pendent_boxes(MsBoxGrid *grid, int direction)
{
	if(direction == MS_BOX_SHIFT_LEFT)
		_shiftLeft(grid);
	else if(direction == MS_BOX_SHIFT_TOP)
		_shiftTop(grid);
	else if(direction == MS_BOX_SHIFT_RIGHT)
		_shiftRight(grid);
	else 
		_shiftDown(grid);
    
    _ms_boxgrid_refresh_borders(grid);
}

// returns 1 if two boxes are with the same color
// todo: check - it can be private
int ms_boxgrid_check_neighbours(MsBoxGrid *g, int y1, int x1, int y2, int x2)
{
    MsBox *box1 = (MsBox*)ms_grid_get_item(g->grid, y1, x1).p;
    MsBox *box2 = (MsBox*)ms_grid_get_item(g->grid, y2, x2).p;
    
    if(box1 == 0 && box2 == 0)
        return 1;
    
    if(box1 == 0 || box2 == 0)
        return 0;
    
    return box1->colorIndex == box2->colorIndex;
}


void ms_boxgrid_unit_test()
{
	int pattern_vert[] = 
	{
		-1,-1,-1,-1,-1,
		-1,-1,-1,-1, 1,
		-1,-1,-1, 1,-1,
		-1,-1,-1, 1, 1,
		-1,-1, 1,-1,-1,
		-1,-1, 1,-1, 1,
		-1,-1, 1, 1,-1,
		-1,-1, 1, 1, 1,
		-1, 1,-1,-1,-1,
		-1, 1,-1,-1, 1,
		-1, 1,-1, 1,-1,
		-1, 1,-1, 1, 1,
		-1, 1, 1,-1,-1,
		-1, 1, 1,-1, 1,
		-1, 1, 1, 1,-1,
		-1, 1, 1, 1, 1,
		 1,-1,-1,-1,-1,
		 1,-1,-1,-1, 1,
		 1,-1,-1, 1,-1,
		 1,-1,-1, 1, 1,
		 1,-1, 1,-1,-1,
		 1,-1, 1,-1, 1,
		 1,-1, 1, 1,-1,
		 1,-1, 1, 1, 1,
		 1, 1,-1,-1,-1,
		 1, 1,-1,-1, 1,
		 1, 1,-1, 1,-1,
		 1, 1,-1, 1, 1,
		 1, 1, 1,-1,-1,
		 1, 1, 1,-1, 1,
		 1, 1, 1, 1,-1,
		 1, 1, 1, 1, 1
		
	};
	int pattern_horiz[] = 
	{
		-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,
		-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,
		-1,-1,-1,-1, 1, 1, 1, 1,-1,-1,-1,-1, 1, 1, 1, 1,-1,-1,-1,-1, 1, 1, 1, 1,-1,-1,-1,-1, 1, 1, 1, 1,
		-1,-1,-1,-1,-1,-1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};
	MsBoxGrid *grid = ms_boxgrid_create_from_pattern(0, pattern_vert, 32, 5, 100.0, 100.0);
	ms_boxgrid_display2(grid);

	//ms_boxgrid_remove_similar_items(grid, 1, 1);
	//ms_boxgrid_remove_similar_items(grid, 4, 0);
	//ms_boxgrid_display2(grid);

	ms_boxgrid_shift_pendent_boxes(grid, MS_BOX_SHIFT_RIGHT);
	ms_boxgrid_display2(grid);
	/*
	ms_boxgrid_shift_pendent_boxes(grid, 1);
	ms_boxgrid_display(grid);

	ms_boxgrid_shift_pendent_boxes(grid, 2);
	ms_boxgrid_display(grid);

	ms_boxgrid_shift_pendent_boxes(grid, 3);
	ms_boxgrid_display(grid);

	/*

	ms_boxgrid_remove_similar_items(grid, 2, 2);
	ms_boxgrid_display(grid);
	ms_boxgrid_remove_similar_items(grid, 2, 0);
	ms_boxgrid_display(grid);
	ms_boxgrid_remove_similar_items(grid, 1, 1);
	ms_boxgrid_display(grid);*/

	ms_boxgrid_free(&grid);
}