#pragma once

#include "../msCommon.h"

class msAnimationBase
{
public:
    GLint m_delayCount;
    GLint m_count;

    static void _defaultStep(msAnimationBase *anim);

public:

    void (*m_performStep)(msAnimationBase*);

    msAnimationBase(GLint delayCount, GLint count, void (*stepCallback)(msAnimationBase*));
    ~msAnimationBase();

    GLint performStep();
};
