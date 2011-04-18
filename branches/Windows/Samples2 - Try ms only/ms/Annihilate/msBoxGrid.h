#pragma once

#include "../msCommon.h"
#include "msBox.h"
#include "msGrid.h"
#include "msPalette.h"
#include <list>

using namespace::std;


#define MS_BOX_SHIFT_DOWN 0
#define MS_BOX_SHIFT_LEFT 1
#define MS_BOX_SHIFT_TOP 2
#define MS_BOX_SHIFT_RIGHT 3

#define MS_BOUNCE_OFFSET 0.002f 

typedef list<msBox*> msBoxList;
typedef msBoxList::iterator msBoxIterator;

class msBoxGrid
{
	msSize size;


	void _ms_boxgrid_refresh_borders();

	GLint ms_boxgrid_check_neighbours(GLint y1, GLint x1, GLint y2, GLint x2);

	GLint* _ms_boxgrid_generate_random_pattern(GLint numRows, GLint numCols, GLint numColors);

	GLint _ms_boxgrid_get_index_by_yx(GLint y, GLint x, GLint numCols);

	void display2();

	void _hiding1(msAnimationBase *a);

	void _removeSimilarBoxes(GLint y, GLint x, GLint c, msBoxList *removedBoxes);

	GLint _ms_boxgrid_has_similar_neighbour(GLint y, GLint x, GLint colorIndex);

	void _exchangeBoxes(GLint y1, GLint x1, GLint y2, GLint x2);

	void _ms_boxgrid_animate_box_hiding(msBoxList *boxes);

	static void _linearFalling(msAnimationBase *anim);

	static void _linearFalling2(msAnimationBase *anim);

	void _exchangeBoxesWithAnimation(GLint y1, GLint x1, GLint y2, GLint x2, GLint direction);

	void _shiftDown();

	void _shiftTop();

	void _shiftLeft();

	void _shiftRight();

public:
	msGrid<msBox*> *grid;

	msPalette *m_palette;

	void init(msPalette *palette, GLint *pattern, GLint numRows, GLint numCols, GLfloat screenHeight, GLfloat screenWidth);

	msBoxGrid(msPalette *palette, GLint *pattern, GLint numRows, GLint numCols, GLfloat screenHeight, GLfloat screenWidth);

	msBoxGrid(msPalette *palette, GLint numColors, GLint numRows, GLint numCols, GLfloat gridHeight, GLfloat gridWidth);

	~msBoxGrid();

	void display();	

	void removeSimilarItems(GLint y, GLint x);

	void shiftPendentBoxes(GLint direction);

	GLint checkNeighbours(GLint y1, GLint x1, GLint y2, GLint x2);

	static void unitTest();

	void removeSimilarItemsAtPoint( msPoint touchPoint );
};
