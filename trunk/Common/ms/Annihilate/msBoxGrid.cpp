#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#include "msBoxGrid.h"
#include "msGrid.h"

void msBoxGrid::_borderInvertion( msAnimationContext *c )
{
    msBorderAnimationContext *context = (msBorderAnimationContext*)c;
    context->updateColor();
}

void msBoxGrid::_animateBorderInvertion( msBox * box, GLint positive ) 
{/*
    GLint count = 500;
    GLfloat step = 1.0f / count;
    if(!positive)
        step *= -1.0;
    msBorderAnimationContext *context = new msBorderAnimationContext(&box->m_border->color, step); 
    msAnimation *animation = new msAnimation(0, count, context, _borderInvertion);
    box->m_animations->m_list.push_back(animation);
    */
}

void msBoxGrid::_ms_boxgrid_refresh_borders()
{
    for(GLint y = 0; y < grid->m_rowCount; y ++)
	{
		for(GLint x = 0; x < grid->m_columnCount; x ++)
		{
			msBox *box = grid->getItem(y, x);

            // left
            GLint left = !ms_boxgrid_check_neighbours(y, x-1, y, x);
            if(left != box->m_border->left)
                _animateBorderInvertion(box, left);           
            box->m_border->left = left;

            // top
            GLint top = !ms_boxgrid_check_neighbours(y-1, x, y, x);
            if(top != box->m_border->top)
                _animateBorderInvertion(box, top);           
            box->m_border->top = top;

            // right
            GLint right = !ms_boxgrid_check_neighbours(y, x, y, x+1);
            if(right != box->m_border->right)
                _animateBorderInvertion(box, right);           
            box->m_border->right = right;

            // bottom
            GLint bottom = !ms_boxgrid_check_neighbours(y, x, y+1, x);
            if(bottom != box->m_border->bottom)
                _animateBorderInvertion(box, bottom);           
            box->m_border->bottom = bottom;
		}
	}
}

msBoxGrid::msBoxGrid(msPalette *palette, GLint *pattern, GLint numRows, GLint numCols, GLfloat gridHeight, GLfloat gridWidth)
{
	init(palette, pattern, numRows, numCols, gridHeight, gridWidth);
}

msBoxGrid::msBoxGrid(msPalette *palette, GLint numColors, GLint numRows, GLint numCols, GLfloat gridHeight, GLfloat gridWidth)
{   
    GLint *pattern = _ms_boxgrid_generate_random_pattern(numRows, numCols, numColors);
	init(palette, pattern, numRows, numCols, gridHeight, gridWidth);
    free(pattern);
}

void msBoxGrid::init(msPalette *palette, GLint *pattern, GLint numRows, GLint numCols, GLfloat gridHeight, GLfloat gridWidth)
{
	float width = gridWidth / numCols, height = gridHeight / numRows;
	float curx = 0, cury = 0;

	grid = new msGrid<msBox*>(numRows, numCols);
	m_palette = palette;

	size.width = gridWidth;
	size.height = gridHeight;

	for(GLint y = 0; y < numRows; y ++)
	{
		curx = 0;
		for(GLint x = 0; x < numCols; x ++)
		{
			msBox *box = new msBox(curx, cury, width, height, pattern[y * numCols + x]);
            if(palette != 0)
                box->m_border->color = *palette->getColor(0);
			grid->setItem(y, x, box);

			curx += width;
		}

		cury += height;
	}

	_ms_boxgrid_refresh_borders();
}


GLint msBoxGrid::_ms_boxgrid_get_index_by_yx(GLint y, GLint x, GLint numCols)
{
	return y * numCols + x;
}

GLint* msBoxGrid::_ms_boxgrid_generate_random_pattern(GLint numRows, GLint numCols, GLint numColors)
{
	int *pattern = (int*)malloc(sizeof(int) * numRows * numCols);
   
	srand((unsigned int)time(0));
    
	for(GLint y = 0; y < numRows; y ++)
		for(GLint x = 0; x < numCols; x ++)
			pattern[_ms_boxgrid_get_index_by_yx(y, x, numCols)] = rand() % numColors + 1;
    
	return pattern;
}



msBoxGrid::~msBoxGrid()
{
	for(GLint y = 0; y < grid->m_rowCount; y ++)
	{
		for(GLint x = 0; x < grid->m_columnCount; x ++)
		{
			msBox *item = grid->getItem(y, x);
			delete item;
		}
	}

	delete grid;
}



void msBoxGrid::display()
{
	for(GLint y = 0; y < grid->m_rowCount; y ++)
	{
		for(GLint x = 0; x < grid->m_columnCount; x ++)
		{
			msBox *box = grid->getItem(y, x);
			if(box->isVisible())
				printf("%d:(%3.0f,%3.0f)  ", box->m_colorIndex, box->m_location.y, box->m_location.x);
			else
				printf("      -      ");
		}
		printf("\r\n");
	}
	printf("\r\n");
}

void msBoxGrid::display2()
{
	for(GLint y = 0; y < grid->m_rowCount; y ++)
	{
		for(GLint x = 0; x < grid->m_columnCount; x ++)
		{
			msBox *box = grid->getItem(y, x);
			if(box->isVisible())
				printf("%d", box->m_colorIndex);
			else
				printf(" ");
		}
		printf("\r\n");
	}
	printf("\r\n");
}


void msBoxGrid::_hiding1(msAnimationContext *c)
{/*
	msAnimation<msBox*> *anim = (msAnimation<msBox*> *)a;
    msBox *box = anim->m_from;
    
    box->m_location.z = 1.0f;
    
    anim->m_rAngle += 1.0f;
    anim->m_rVector.x = 1.0f;
    anim->m_rVector.y = 0.0f;
    anim->m_rVector.z = 0.0f;*/
}




void msBoxGrid::_ms_boxgrid_animate_box_hiding(msBoxList *boxes)
{    
    //ms_box_copy(box, box->boxToAnimate);
    
    // add animation
   // MsAnimation *sizeAnimation = ms_animation_create(_ptog(box->boxToAnimate), _itog(100), 0, 100, _hiding1);
    //ms_list_add_item(box->animations->list, _ptog(sizeAnimation));    
    
    //sizeAnimation = ms_animation_create(_ptog(&box->boxToAnimate->location), _itog(100), 10, 20, _hiding2);
    // ms_list_add_item(box->animations->list, _ptog(sizeAnimation));
}



void msBoxGrid::_removeSimilarBoxes(GLint y, GLint x, GLint c, msBoxList *removedBoxes)
{
    // out of range
    if(x < 0 || y < 0 || x >= grid->m_columnCount || y >= grid->m_rowCount)
        return;
    
    msBox *box = grid->getItem(y, x);
	if(!box->isVisible())
		return;
    
    // if the neighbour has different colour
    if(box->m_colorIndex != c)
        return;
    
	// make the box invisible
	box->makeInvisible();
    
    // add it to list
	removedBoxes->push_back(box);
       
    _removeSimilarBoxes(y, x-1, c, removedBoxes);
    _removeSimilarBoxes(y-1, x, c, removedBoxes);
    _removeSimilarBoxes(y, x+1, c, removedBoxes);
    _removeSimilarBoxes(y+1, x, c, removedBoxes);
}


int msBoxGrid::_ms_boxgrid_has_similar_neighbour(GLint y, GLint x, GLint colorIndex)
{
    msBox *neighbour;
    
    if(x < 0 || x >= grid->m_columnCount || y < 0 || y >= grid->m_rowCount)
        return 0;
    
    if(x-1 >= 0)
    {
        neighbour = grid->getItem(y, x-1);
        if(neighbour->m_colorIndex == colorIndex)
            return 1;
    }
    if(x+1 < grid->m_columnCount)
    {
        neighbour = grid->getItem(y, x+1);
        if(neighbour->m_colorIndex == colorIndex)
            return 1;
    }
    if(y-1 >= 0)
    {
        neighbour = grid->getItem(y-1, x);
        if(neighbour->m_colorIndex == colorIndex)
            return 1;
    }
    if(y+1 < grid->m_rowCount)
    {
        neighbour = grid->getItem(y+1, x);
        if(neighbour->m_colorIndex == colorIndex)
            return 1;
    }
    return 0;    
}


void msBoxGrid::removeSimilarItems(GLint y, GLint x)
{
	msBoxList removedBoxes;
	msBox *box = grid->getItem(y, x);
	if(box->isVisible())
    {
        if(_ms_boxgrid_has_similar_neighbour(y, x, box->m_colorIndex))
        {           
            _removeSimilarBoxes(y, x, box->m_colorIndex, &removedBoxes);
           
            _ms_boxgrid_animate_box_hiding(&removedBoxes);
        }
    }
}


void msBoxGrid::_exchangeBoxes(GLint y1, GLint x1, GLint y2, GLint x2)
{
	msBox *box1 = grid->getItem(y1, x1);
	msBox *box2 = grid->getItem(y2, x2);

	grid->setItem(y1, x1, box2);
	grid->setItem(y2, x2, box1);
}

void msBoxGrid::_linearFalling(msAnimationContext *c)
{
	msFromToAnimationContext<msPoint*> *context = (msFromToAnimationContext<msPoint*> *)c;

	msPoint *from = context->getFrom();
	msPoint *to = context->getTo();
	
	from->x += (to->x - from->x) / context->getAnimation()->getCount();
	from->y += (to->y - from->y) / context->getAnimation()->getCount();
}

void msBoxGrid::_linearFalling2(msAnimationContext *c)
{
	msPointMoveAnimationContext *context = (msPointMoveAnimationContext*)c;
	msPoint *p = context->getPoint();
    switch(context->getDirection())
    {
        case MS_BOX_SHIFT_TOP:
            p->y += MS_BOUNCE_OFFSET;
            break;
            
        case MS_BOX_SHIFT_LEFT:
            p->x += MS_BOUNCE_OFFSET;
            break;
            
        case MS_BOX_SHIFT_RIGHT:
            p->x -= MS_BOUNCE_OFFSET;
            break;
            
        default:
            p->y -= MS_BOUNCE_OFFSET;
    }    
}


void msBoxGrid::_exchangeBoxesWithAnimation(GLint y1, GLint x1, GLint y2, GLint x2, GLint direction)
{
	msBox *box1 = grid->getItem(y1, x1);
	msBox *box2 = grid->getItem(y2, x2);
    
	grid->setItem(y1, x1, box2);
	grid->setItem(y2, x2, box1);
    
    msPoint tempLocation = box1->m_location;
    box1->m_location = box2->m_location;
    box2->m_location = tempLocation;
    
    box1->m_boxToAnimate->m_border = box1->m_border;
  
    int times = 10;

    // moving from one position to another
	msAnimation *animation = new msAnimation(0, times, new msFromToAnimationContext<msPoint*>(&box1->m_boxToAnimate->m_location, &box1->m_location), _linearFalling);
	box1->m_animations->m_list.push_back(animation);
    
    animation = new msAnimation(times, 4, new msPointMoveAnimationContext(&box1->m_boxToAnimate->m_location, direction), _linearFalling2);
	box1->m_animations->m_list.push_back(animation);
    
    animation = new msAnimation(times + 4, 1, new msFromToAnimationContext<msPoint*>(&box1->m_boxToAnimate->m_location, &box1->m_location), _linearFalling);
	box1->m_animations->m_list.push_back(animation);
}

void msBoxGrid::_shiftDown()
{
	for(GLint x = 0; x < grid->m_columnCount; x ++)
	{
		GLint y = grid->m_rowCount - 1;
		while(y >= -1)
		{
			if(!grid->getItem(y, x)->isVisible())
			{
				// y < 0 when there are no empty boxes
				if(y < 0)
					break;

				GLint yy = y - 1; // note: dependent on direction
				while(yy >= -1 && !grid->getItem(yy, x)->isVisible())
					yy --;

				// yy < -1 when there are no items to shift
				if(yy <= -1)
					break;
                
				// shift found box
				_exchangeBoxesWithAnimation(yy, x, y, x, MS_BOX_SHIFT_DOWN);
			}
			
			y --;
		}
	}
}


void msBoxGrid::_shiftTop()
{  
	for(GLint x = 0; x < grid->m_columnCount; x ++)
	{
		GLint y = 0;
		while(y < grid->m_rowCount)
		{
			if(!grid->getItem(y, x)->isVisible())
			{
				// y < 0 when there are no empty boxes
				if(y >= grid->m_rowCount)
					break;

				GLint yy = y + 1; // note: dependent on direction
				while(yy <= grid->m_rowCount && !grid->getItem(yy, x)->isVisible())
					yy ++;

				// yy < -1 when there are no items to shift
				if(yy > grid->m_rowCount - 1)
					break;

				// shift found box
				_exchangeBoxesWithAnimation(yy, x, y, x, MS_BOX_SHIFT_TOP);
			}
			
			y ++;
		}
	}
}


void msBoxGrid::_shiftRight()
{
	for(GLint y = 0; y < grid->m_rowCount; y ++)
	{
		GLint x = grid->m_columnCount - 1;
		while(x >= -1)
		{
			if(!grid->getItem(y, x)->isVisible())
			{
				// y < 0 when there are no empty boxes
				if(x < 0)
					break;

				GLint xx = x - 1; // note: dependent on direction
				while(xx >= -1 && !grid->getItem(y, xx)->isVisible())
					xx --;

				// yy < -1 when there are no items to shift
				if(xx <= -1)
					break;

				// shift found box
				_exchangeBoxesWithAnimation(y, xx, y, x, MS_BOX_SHIFT_RIGHT);
			}
			
			x --;
		}
	}
}


void msBoxGrid::_shiftLeft()
{
	for(GLint y = 0; y < grid->m_rowCount; y ++)
	{
		GLint x = 0;
		while(x < grid->m_columnCount)
		{
			if(!grid->getItem(y, x)->isVisible())
			{
				// y < 0 when there are no empty boxes
				if(x >= grid->m_columnCount)
					break;

				GLint xx = x + 1; // note: dependent on direction
				while(xx <= grid->m_columnCount && !grid->getItem(y, xx)->isVisible())
					xx ++;

				// yy < -1 when there are no items to shift
				if(xx > grid->m_columnCount - 1)
					break;

				// shift found box
				_exchangeBoxesWithAnimation(y, xx, y, x, MS_BOX_SHIFT_LEFT);
			}
			
			x ++;
		}
	}
}


void msBoxGrid::shiftPendentBoxes(GLint direction)
{
	if(direction == MS_BOX_SHIFT_LEFT)
		_shiftLeft();
	else if(direction == MS_BOX_SHIFT_TOP)
		_shiftTop();
	else if(direction == MS_BOX_SHIFT_RIGHT)
		_shiftRight();
	else 
		_shiftDown();
    
    _ms_boxgrid_refresh_borders();
}

// returns 1 if two boxes are with the same color
// todo: check - it can be private
int msBoxGrid::ms_boxgrid_check_neighbours(GLint y1, GLint x1, GLint y2, GLint x2)
{
    msBox *box1 = grid->getItem(y1, x1);
    msBox *box2 = grid->getItem(y2, x2);
    
    if(box1 == 0 && box2 == 0)
        return 1;
    
    if(box1 == 0 || box2 == 0)
        return 0;
    
    return box1->m_colorIndex == box2->m_colorIndex;
}


void msBoxGrid::unitTest()
{
	GLint pattern_vert[] = 
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
	GLint pattern_horiz[] = 
	{
		-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,
		-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,
		-1,-1,-1,-1, 1, 1, 1, 1,-1,-1,-1,-1, 1, 1, 1, 1,-1,-1,-1,-1, 1, 1, 1, 1,-1,-1,-1,-1, 1, 1, 1, 1,
		-1,-1,-1,-1,-1,-1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};
	//msBoxGrid grid(0, pattern_vert, 32, 5, 100.0, 100.0);
	msBoxGrid grid(0, pattern_horiz, 5, 32, 100.0, 100.0);
	grid.display2();

	grid.removeSimilarItems(1, 1);
	grid.removeSimilarItems(0, 31);
	grid.display2();

	grid.shiftPendentBoxes(MS_BOX_SHIFT_RIGHT);
	grid.display2();
	/*
	ms_boxgrid_shift_pendent_boxes(grid, 1);
	ms_boxgrid_display(grid);

	ms_boxgrid_shift_pendent_boxes(grid, 2);
	ms_boxgrid_display(grid);

	ms_boxgrid_shift_pendent_boxes(grid, 3);
	ms_boxgrid_display(grid);

	

	//ms_boxgrid_remove_similar_items(grid, 2, 2);
	//ms_boxgrid_display(grid);
	//ms_boxgrid_remove_similar_items(grid, 2, 0);
	//ms_boxgrid_display(grid);
	//ms_boxgrid_remove_similar_items(grid, 1, 1);
	//ms_boxgrid_display(grid);
*/
}


void msBoxGrid::removeSimilarItemsAtPoint( msPoint screenPoint )
{
	msPoint point;
	point.x = screenPoint.x * this->size.width;
	point.y = screenPoint.y * this->size.height;

	for(int y = 0; y < grid->m_rowCount; y ++)
	{
		for(int x = 0; x < grid->m_columnCount; x ++)
		{
			msBox *box = grid->getItem(y, x);

			if((box->m_location.x <= point.x && point.x <= box->m_location.x + box->m_size.width) &&
			   (box->m_location.y <= point.y && point.y <= box->m_location.y + box->m_size.height))
			{
				removeSimilarItems(y, x);
				return;
			}
		}
	}
}