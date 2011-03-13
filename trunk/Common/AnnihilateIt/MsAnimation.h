
#ifndef _MsAnimation_H
#define _MsAnimation_H


#include "MsCommon.h"



// represents animation unit
typedef struct MsAnimation
{
	// object to animate from
	generic from;

	// object to animate to
	generic to;
    
    // number of times the performSteps will do nothing before actual work
    // (0 to start animation immidiately)
    int delayCount;

	// number of times the performSteps will do work
	int count;
    
    
    

    
    float rAngle;
    point rVector;
    

	// the call implements a way the animation value has to be changed with every step
	void (*performStep)(struct MsAnimation*);
} MsAnimation;



MsAnimation* ms_animation_create_default(float from, float to, int delayCount, int count);

MsAnimation* ms_animation_create(generic from, generic to, int delayCount, int count, void (*stepCallback)(MsAnimation*));

int ms_animation_perform_step(MsAnimation *animation);

void ms_animation_free(MsAnimation **animation);

void ms_animation_unit_test();


#endif