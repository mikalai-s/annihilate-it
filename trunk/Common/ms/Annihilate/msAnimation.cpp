#include "msAnimation.h"
#include "msAnimationContext.h"

msAnimation::msAnimation(GLint delayCount, GLint count, msAnimationContext *context, void (*stepCallback)(msAnimationContext*))
{
    this->delayCount = delayCount;
    this->count = count;
    this->context = context;
    this->performStepCallback = (stepCallback == 0) ? _defaultStep : stepCallback;

    this->context->animation = this;
}

msAnimation::~msAnimation()
{
    delete this->context;
}

void msAnimation::_defaultStep(msAnimationContext *context)
{
    msFromToAnimationContext<GLfloat> *c = (msFromToAnimationContext<GLfloat>*)context;
    GLfloat step = (c->getTo() - c->getFrom()) / c->getAnimation()->getCount();
    c->setFrom(c->getFrom() + step);
}

// NOTE:    don't forget to delete animation object after the last perform step has executed
GLint msAnimation::performStep()
{
    if(this->performStepCallback == 0)
        return 0;

    if(this->count <= 0)
        return 0;

    if(this->delayCount > 0)
    {
        this->delayCount --;
        return 1;
    }

    this->performStepCallback(this->context);

    this->count --;

    return (this->count >= 0);
}

msAnimationContext* msAnimation::getContext()
{
    return this->context;
}

GLint msAnimation::getCount()
{
    return this->count;
}

void msAnimation::_unitTestLineStep(msAnimationContext *c)
{
    msFromToAnimationContext<msPoint3f*> *context = (msFromToAnimationContext<msPoint3f*>*)c;
    msPoint3f *from = context->getFrom();
    msPoint3f *to = context->getTo();
    msPoint3f step;
    step.x = (to->x - from->x) / context->getAnimation()->getCount();
    step.y = (to->y - from->y) / context->getAnimation()->getCount();

    from->x += step.x;
    from->y += step.y;
}

void msAnimation::unitTest()
{
    msPoint3f from;
    msPoint3f to;
    int count = 10;

    from.x = 0;
    from.y = 0;

    to.x = 10;
    to.y = 5;

    msFromToAnimationContext<msPoint3f*> *context = new msFromToAnimationContext<msPoint3f*>(&from, &to);
    msAnimation *animation = new msAnimation(0, count, context, _unitTestLineStep);

    do
    {
        printf("(%f, %f)\r\n", context->getFrom()->x, context->getFrom()->y);
    }
    while(animation->performStep());

    delete animation;
}