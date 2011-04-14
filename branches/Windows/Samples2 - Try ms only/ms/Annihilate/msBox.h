#pragma once

#include "msAnimationBundle.h"
#include "msAnimation.h"

#define MS_BOX_INVISIBLE -1

class msBox
{
public:
	msPoint m_location;
	msSize m_size;
	int m_colorIndex;
	//    color color;

	// copy of the current box. is used just for animation. can be refactored in the future.
	msBox *m_boxToAnimate;

	// animations related
	msAnimationBundle *m_animations;

	// visibility of border
	msBorder m_border;

private:
	msBox();

public:
	msBox(float x, float y, float width, float height, int colorIndex);

	~msBox();

	msBox* copy(msBox *target);

	void makeInvisible();

	GLboolean isVisible();

//	void unitTest();

};