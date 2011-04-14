#pragma once

#include "MsAnimationBundle.h"
#include "MsAnimation.h"

#define MS_BOX_INVISIBLE -1

class MsBox
{
public:
	msPoint m_location;
	msSize m_size;
	int m_colorIndex;
	//    color color;

	// copy of the current box. is used just for animation. can be refactored in the future.
	MsBox *m_boxToAnimate;

	// animations related
	MsAnimationBundle *m_animations;

	// visibility of border
	msBorder m_border;

private:
	MsBox();

public:
	MsBox(float x, float y, float width, float height, int colorIndex);

	~MsBox();

	MsBox* copy(MsBox *target);

	void makeInvisible();

	GLboolean isVisible();

//	void unitTest();

};