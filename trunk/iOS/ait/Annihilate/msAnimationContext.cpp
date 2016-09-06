#include "msAnimationContext.h"

msAnimationContext::msAnimationContext(void *owner)
{
    this->owner = owner;
}

msAnimationContext::~msAnimationContext(void)
{
}

msAnimation* msAnimationContext::getAnimation()
{
    return this->animation;
}

void* msAnimationContext::getOwner()
{
    return this->owner;
}



msPointMoveAnimationContext::msPointMoveAnimationContext(void *owner, msPoint3f *point, GLint direction) : msAnimationContext(owner)
{
    this->point = point;
    this->direction = direction;
}

msPointMoveAnimationContext::~msPointMoveAnimationContext()
{
}

msPoint3f* msPointMoveAnimationContext::getPoint()
{
    return this->point;
}

GLint msPointMoveAnimationContext::getDirection()
{
    return this->direction;
}