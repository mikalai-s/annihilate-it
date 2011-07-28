#pragma once

#include "../msCommon.h"

class msAnimationContext;

class msAnimation
{
private:
    int delayCount;
    int count;

    msAnimationContext *context;

    static void _defaultStep(msAnimationContext *c);

    static void _unitTestLineStep(msAnimationContext *c);

public:

    void (*performStepCallback)(msAnimationContext*);

    msAnimation(GLint delayCount, GLint count, msAnimationContext *context, void (*stepCallback)(msAnimationContext*));
    ~msAnimation();
    
    int getDelayCount();
    
    int getCount();

    msAnimationContext *getContext();

    GLint performStep();
    
    // stops current animation
    void stop();

    static void unitTest();
};


