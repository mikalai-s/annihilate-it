#pragma once

#include "msAnimationBundle.h"
#include "msAnimation.h"

#define MS_BOX_INVISIBLE -1

class msBoxAnimation;

class msBox
{
public:
	msPoint m_location;
	msSize m_size;
	int m_colorIndex;
	msColor m_colorDisturbance;

    // visibility of border
	msBorder *m_border;

private:
	// copy of the current box. is used just for animation. can be refactored in the future.
	msBoxAnimation *m_boxToAnimate;	

protected:
	msBox();

public:
	msBox(float x, float y, float width, float height, int colorIndex);

	~msBox();

	msBox* copy(msBox *target);

	void makeInvisible();

	GLboolean isVisible();

    msBoxAnimation *getAnimated();

	static void unitTest();

};