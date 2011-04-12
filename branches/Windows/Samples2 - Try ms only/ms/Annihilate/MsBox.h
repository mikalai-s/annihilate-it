
#ifndef _MsBox_H
#define _MsBox_H

#include "MsAnimationBundle.h"
#include "MsAnimation.h"
#include "MsList.h"

#define MS_BOX_INVISIBLE -1

typedef struct MsBox
{
	// appearance
   	point location;
	size size;
	int colorIndex;
//    color color;
    
    // copy of the current box. is used just for animation. can be refactored in the future.
    struct MsBox *boxToAnimate;

	// animations related
	MsAnimationBundle *animations;
    
    // visibility of border
    border border;
} MsBox;

MsBox* ms_box_create(float x, float y, float width, float height, int colorIndex);

void ms_box_free(MsBox **box);

void ms_box_copy(MsBox *b1, MsBox *b2);

void ms_box_make_invisible(MsBox *box);

int ms_box_is_visible(MsBox *box);

void ms_box_unit_test();

#endif