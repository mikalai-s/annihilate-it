#pragma once

#include "../msCommon.h"
#include "msAnimation.h"

class msAnimationContext
{
    friend class msAnimation;

    msAnimation *animation;
    void *owner;

public:
    msAnimationContext(void *owner);
    ~msAnimationContext(void);

    msAnimation* getAnimation();
    void *getOwner();    
};

template <class T>
class msFromToAnimationContext : public msAnimationContext
{
    T from;
    T to;

public:
    msFromToAnimationContext(void *owner, T from, T to) : msAnimationContext(owner)
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
    msPointMoveAnimationContext(void *owner, msPoint3f *point, GLint direction);
    ~msPointMoveAnimationContext();

    msPoint3f* getPoint();
    GLint getDirection();
};

template <class T>
class msValueAnimationContext: public msAnimationContext
{
    T value;

public:
    msValueAnimationContext(void *owner, T value) : msAnimationContext(owner)
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
    msKeyValueAnimationContext(void *owner, TKey key, TValue value) : msAnimationContext(owner)
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