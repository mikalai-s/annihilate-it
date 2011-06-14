#pragma once

#include "../msCommon.h"
#include "msBox.h"
#include "msGrid.h"
#include "msPalette.h"
#include <list>
#include <map>


using namespace::std;


#define MS_BOX_SHIFT_DOWN 0
#define MS_BOX_SHIFT_LEFT 1
#define MS_BOX_SHIFT_TOP 2
#define MS_BOX_SHIFT_RIGHT 3

#define MS_BOUNCE_OFFSET 0.002f 


#define MS_WAVE_DELAY 5


typedef list<msBox*> msBoxList;
typedef msBoxList::iterator msBoxIterator;

typedef map<msBox*, int> msBoxExplMap;
typedef msBoxExplMap::iterator msBoxExplIterator;

class msBoxGrid : public msGrid<msBox*>
{
	msSize size;

	void _refreshBorders();

    static void _borderInvertion( msAnimationContext *context );

    void _animateBorderInvertion( msBox * box, GLint positive );

	GLint ms_boxgrid_check_neighbours(GLint y1, GLint x1, GLint y2, GLint x2);

	GLint* _generate_random_pattern(GLint numRows, GLint numCols, GLint numColors);

	void display2();

	static void _hiding1(msAnimationContext *c);

	void _removeSimilarBoxes(GLint y, GLint x, GLint c, msBoxExplMap &removedBoxes, GLint level);

	GLint _checkBoxColor(GLint y, GLint x, GLint colorIndex);

	void _exchangeBoxes(GLint y1, GLint x1, GLint y2, GLint x2);

	void _animateBoxHiding(msBoxExplMap &boxes);

	void _exchangeBoxesWithAnimation(GLint y1, GLint x1, GLint y2, GLint x2, GLint direction);

	void _shiftDown();

	void _shiftTop();

	void _shiftLeft();

	void _shiftRight();

	msAnimationBundle m_animations;

	static void doBoxesFallingCallback(msAnimationContext *c);

public:

	msPalette *m_palette;

	msAnimationBundle *getAnimations();

	void init(msPalette *palette, GLint *pattern, GLint numRows, GLint numCols, GLfloat gridHeight, GLfloat gridWidth);

	msBoxGrid(msPalette *palette, GLint *pattern, GLint numRows, GLint numCols, GLfloat gridHeight, GLfloat gridWidth);

	msBoxGrid(msPalette *palette, GLint numColors, GLint numRows, GLint numCols, GLfloat gridHeight, GLfloat gridWidth);

	~msBoxGrid();

	void display();	

	void removeSimilarItems(GLint y, GLint x);

	void shiftPendentBoxes(GLint direction);

	GLint checkNeighbours(GLint y1, GLint x1, GLint y2, GLint x2);

	static void unitTest();

	void removeSimilarItemsAtPoint( msPoint touchPoint );

	// goes through boxes and updates links between them
	void _updateLinks();
	void undo();
};

class msBorderAnimationContext : public msAnimationContext
{
    msColor *m_color;
    GLfloat m_step;

public:
    msBorderAnimationContext(msColor *color, GLfloat step)
    {
        m_color = color;
        m_step = step;
    }

    void updateColor()
    {
        /*m_color->r += m_step;
        m_color->g += m_step;
        m_color->b += m_step;*/
        m_color->a += m_step;
    }
};
