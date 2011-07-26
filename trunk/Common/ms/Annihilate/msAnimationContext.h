#pragma once

#include "../msCommon.h"
#include "msAnimation.h"

class msAnimationContext
{
    friend class msAnimation;

    msAnimation *animation;

public:
    msAnimationContext(void);
    ~msAnimationContext(void);

    msAnimation* getAnimation();
};

template <class T>
class msFromToAnimationContext : public msAnimationContext
{
    T from;
    T to;

public:
    msFromToAnimationContext(T from, T to)
    {
        this->from = from;
        this->to = to;
    }

    ~msFromToAnimationContext()
    {
    }

    T getFrom()
    {
        return this->from;
    }

    void setFrom(T value)
    {
        this->from = value;
    }

    T getTo()
    {
        return this->to;
    }

    void setTo(T value)
    {
        this->to = value;
    }
};

class msPointMoveAnimationContext : public msAnimationContext
{
    msPoint3f *point;
    GLint direction;

public:
    msPointMoveAnimationContext(msPoint3f *point, GLint direction);
    ~msPointMoveAnimationContext();

    msPoint3f* getPoint();
    GLint getDirection();
};

template <class T>
class msValueAnimationContext: public msAnimationContext
{
    T value;

public:
    msValueAnimationContext(T value)
    {
        this->value = value;
    }

    ~msValueAnimationContext()
    {
    }

    T getValue()
    {
        return this->value;
    }
};

template <class TKey, class TValue>
class msKeyValueAnimationContext: public msAnimationContext
{
    TKey key;
    TValue value;

public:
    msKeyValueAnimationContext(TKey key, TValue value)
    {
        this->key = key;
        this->value = value;
    }

    ~msKeyValueAnimationContext()
    {
    }

    TKey getKey()
    {
        return this->key;
    }

    TValue getValue()
    {
        return this->value;
    }
};