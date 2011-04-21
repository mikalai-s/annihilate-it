#pragma once

#include "../msCommon.h"
#include "msAnimation.h"

class msAnimationContext
{
    friend class msAnimation;

    msAnimation *m_animation;

public:
    msAnimationContext(void);
    ~msAnimationContext(void);

    msAnimation* getAnimation();
};

template <class T>
class msFromToAnimationContext : public msAnimationContext
{
    T m_from;
    T m_to;

public:
    msFromToAnimationContext(T from, T to)
    {
        m_from = from;
        m_to = to;
    }

    ~msFromToAnimationContext()
    {
    }

    T getFrom()
    {
        return m_from;
    }

    void setFrom(T value)
    {
        m_from = value;
    }

    T getTo()
    {
        return m_to;
    }

    void setTo(T value)
    {
        m_to = value;
    }
};

class msPointMoveAnimationContext : public msAnimationContext
{
    msPoint *m_point;
    GLint m_direction;

public:
    msPointMoveAnimationContext(msPoint *point, GLint direction);
    ~msPointMoveAnimationContext();

    msPoint* getPoint();
    GLint getDirection();
};

template <class T>
class msValueAnimationContext: public msAnimationContext
{
    T m_value;

public:
    msValueAnimationContext(T value)
    {
        m_value = value;
    }

    ~msValueAnimationContext()
    {
    }

    T getValue()
    {
        return m_value;
    }
};