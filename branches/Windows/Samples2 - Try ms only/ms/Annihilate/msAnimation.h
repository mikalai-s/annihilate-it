#pragma once

#include "../msCommon.h"

class msAnimationContext;

class msAnimation
{
private:
    GLint m_delayCount;
    GLint m_count;

    msAnimationContext *m_context;

    static void _defaultStep(msAnimationContext *c);

    static void _unitTestLineStep(msAnimationContext *c);

public:

    void (*m_performStepCallback)(msAnimationContext*);

    msAnimation(GLint delayCount, GLint count, msAnimationContext *context, void (*stepCallback)(msAnimationContext*));
    ~msAnimation();

    msAnimationContext *getContext();

    GLint getCount();

    GLint performStep();

    static void unitTest();
};


