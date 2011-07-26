#include "msAnimationContext.h"

msAnimationContext::msAnimationContext(void)
{
}

msAnimationContext::~msAnimationContext(void)
{
}

msAnimation* msAnimationContext::getAnimation()
{
    return this->animation;
}



msPointMoveAnimationContext::msPointMoveAnimationContext(msPoint3f *point, GLint direction)
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