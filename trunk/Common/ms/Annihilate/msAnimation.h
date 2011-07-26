#pragma once

#include "../msCommon.h"

class msAnimationContext;

class msAnimation
{
private:
    GLint delayCount;
    GLint count;

    msAnimationContext *context;

    static void _defaultStep(msAnimationContext *c);

    static void _unitTestLineStep(msAnimationContext *c);

public:

    void (*performStepCallback)(msAnimationContext*);

    msAnimation(GLint delayCount, GLint count, msAnimationContext *context, void (*stepCallback)(msAnimationContext*));
    ~msAnimation();

    msAnimationContext *getContext();

    GLint getCount();

    GLint performStep();

    static void unitTest();
};


