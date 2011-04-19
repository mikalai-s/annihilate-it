#include "msAnimationContext.h"

msAnimationContext::msAnimationContext(void)
{
}

msAnimationContext::~msAnimationContext(void)
{
}

msAnimation* msAnimationContext::getAnimation()
{
    return m_animation;
}



msPointMoveAnimationContext::msPointMoveAnimationContext(msPoint *point, GLint direction)
{
    m_point = point;
    m_direction = direction;
}

msPointMoveAnimationContext::~msPointMoveAnimationContext()
{
}

msPoint* msPointMoveAnimationContext::getPoint()
{
    return m_point;
}

GLint msPointMoveAnimationContext::getDirection()
{
    return m_direction;
}