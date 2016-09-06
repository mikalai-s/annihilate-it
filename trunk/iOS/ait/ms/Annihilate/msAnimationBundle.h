#pragma once

#include "msAnimation.h"
#include "msAnimationContext.h"
#include "../MsCommon.h"
#include <list>

using namespace::std;

typedef std::list<msAnimation*>    msAnimationList;
typedef msAnimationList::iterator    msAnimationIterator;

// represents set of animations each animation autoreleases when it's done
class msAnimationBundle
{
    msAnimationList list;

public:
    static void _lineStep2(msAnimationContext *context);

public:
    msAnimationBundle();
    ~msAnimationBundle();

    void performStep();

    GLint getCount();
    
    void add(msAnimation *animation);
    
    void addAnimation(GLint delayCount, GLint count, msAnimationContext *context, void (*stepCallback)(msAnimationContext*));
    
    void addSerialAnimation(GLint count, msAnimationContext *context, void (*stepCallback)(msAnimationContext*));

    msAnimationIterator getFirst();

    msAnimationIterator getLast();

    static void unitTest();
    
    
    // unitility functions:    
    template <class T>
    static void _setValueCallback(msAnimationContext *c)
    {
        msKeyValueAnimationContext<T, GLfloat> *context = (msKeyValueAnimationContext<T, GLfloat>*)c;
        *(context->getKey()) = context->getValue();
    }
    
    template <class T>
    void setValueLazily(GLint delay, T *flag, T value)
    {
        msKeyValueAnimationContext<T*, GLfloat> *context = new msKeyValueAnimationContext<T*, GLfloat>(0, flag, value);
        this->addAnimation(delay, 1, context, _setValueCallback<T*>);
    }
    
    template <class T>
    void setSerialValueLazily(T *flag, T value)
    {
        msKeyValueAnimationContext<T*, GLfloat> *context = new msKeyValueAnimationContext<T*, GLfloat>(0, flag, value);
        this->addSerialAnimation(1, context, _setValueCallback<T*>);
    }
};