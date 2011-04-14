#pragma once

#include "MsBox.h"
#include "MsGrid.h"
#include "MsPalette.h"
#include <list>

using namespace::std;


#define MS_BOX_SHIFT_DOWN 0
#define MS_BOX_SHIFT_LEFT 1
#define MS_BOX_SHIFT_TOP 2
#define MS_BOX_SHIFT_RIGHT 3

typedef list<MsBox*> msBoxList;
typedef msBoxList::iterator msBoxIterator;

class MsBoxGrid
{
	msSize size;

	msGrid<MsBox*> *grid;

	msPalette *palette;

	void _ms_boxgrid_refresh_borders();

	int ms_boxgrid_check_neighbours(GLuint y1, GLuint x1, GLuint y2, GLuint x2);

	GLint* _ms_boxgrid_generate_random_pattern(GLuint numRows, GLuint numCols, GLuint numColors);

	int _ms_boxgrid_get_index_by_yx(GLuint y, GLuint x, GLuint numCols);

	void display2();

	void _hiding1(msAnimationBase *a);

	void _removeSimilarBoxes(GLuint y, GLuint x, GLuint c, msBoxList *removedBoxes);

	int _ms_boxgrid_has_similar_neighbour(GLuint y, GLuint x, GLuint colorIndex);

	void _exchangeBoxes(GLuint y1, GLuint x1, GLuint y2, GLuint x2);

	void _ms_boxgrid_animate_box_hiding(msBoxList *boxes);

	static void _linearFalling(msAnimationBase *anim);

	static void _linearFalling2(msAnimationBase *anim);

	void _exchangeBoxesWithAnimation(GLuint y1, GLuint x1, GLuint y2, GLuint x2, int direction);

	void _shiftDown();

	void _shiftTop();

	void _shiftLeft();

	void _shiftRight();

public:
	void init(msPalette *palette, GLint *pattern, GLuint numRows, GLuint numCols, GLfloat screenHeight, GLfloat screenWidth);

	MsBoxGrid(msPalette *palette, GLint *pattern, GLuint numRows, GLuint numCols, GLfloat screenHeight, GLfloat screenWidth);

	MsBoxGrid(msPalette *palette, GLuint numColors, GLuint numRows, GLuint numCols, GLfloat gridHeight, GLfloat gridWidth);

	~MsBoxGrid();

	void display();	

	void removeSimilarItems(GLuint y, GLuint x);

	void shiftPendentBoxes(GLint direction);

	GLint checkNeighbours(GLint y1, GLint x1, GLint y2, GLint x2);

	void unitTest();
};
