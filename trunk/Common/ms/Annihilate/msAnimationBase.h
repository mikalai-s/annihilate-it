#pragma once

#include "../msCommon.h"

class msAnimationBase
{
public:
    GLint delayCount;
    GLint count;

    static void _defaultStep(msAnimationBase *anim);

public:

    void (*performStep)(msAnimationBase*);

    msAnimationBase(GLint delayCount, GLint count, void (*stepCallback)(msAnimationBase*));
    ~msAnimationBase();

    GLint performStep();
};
