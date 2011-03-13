
#ifndef _MsAnimationBundle_H
#define _MsAnimationBundle_H

#include "MsAnimation.h"
#include "MsList.h"
#include "MsCommon.h"


// represents set of animations each animation autoreleases when it's done
typedef struct MsAnimationBundle
{
	// list of animations
	MsList *list;
} MsAnimationBundle;



MsAnimationBundle* ms_animationbundle_create();

void ms_animationbundle_perform_step(MsAnimationBundle *bundle);

void ms_animationbundle_free(MsAnimationBundle **bundle);

void ms_animationbundle_unit_test();


#endif