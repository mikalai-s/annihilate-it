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



msPointMoveAnimationContext::msPointMoveAnimationContext(msPointf *point, GLint direction)
{
    m_point = point;
    m_direction = direction;
}

msPointMoveAnimationContext::~msPointMoveAnimationContext()
{
}

msPointf* msPointMoveAnimationContext::getPoint()
{
    return m_point;
}

GLint msPointMoveAnimationContext::getDirection()
{
    return m_direction;
}