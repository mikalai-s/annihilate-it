#include "msAnimationBundle.h"
#include "msAnimation.h"
#include "stdlib.h"
#include "stdio.h"

msAnimationBundle::msAnimationBundle()
{
}

void msAnimationBundle::performStep()
{
    // perform next animation step
    msAnimationList animationsToDelete;
    for(msAnimationIterator i = this->list.begin(); i != this->list.end(); i ++)
    {
        msAnimation *animation = *i;
        if(animation->performStep() == 0)
        {
            animationsToDelete.push_back(animation);
        }
    }

    // delete expired animations
    for(msAnimationIterator i = animationsToDelete.begin(); i != animationsToDelete.end(); i ++)
    {
        this->list.remove(*i);
        delete (*i);
    }
}

msAnimationBundle::~msAnimationBundle()
{
    while(this->list.size() > 0)
    {
        msAnimation *a = this->list.front();
        this->list.remove(a);
        delete a;
    }
}

void msAnimationBundle::_lineStep2(msAnimationContext *context)
{
    msFromToAnimationContext<GLfloat*> *c = (msFromToAnimationContext<GLfloat*>*)context;
    GLfloat *from = c->getFrom();
    GLfloat *to = c->getTo();
    *from += (*to - *from) / c->getAnimation()->getCount();
}

void msAnimationBundle::unitTest()
{
    GLfloat from1 = 0, from2 = 1;
    GLint times1 = 5, times2 = 10;
    GLfloat to1 = 2.5, to2 = 7;

    msAnimation *a1 = new msAnimation(0, times1, new msFromToAnimationContext<GLfloat*>(0, &from1, &to1), _lineStep2);
    msAnimation *a2 = new msAnimation(0, times2, new msFromToAnimationContext<GLfloat*>(0, &from2, &to2), _lineStep2);

    msAnimationBundle bundle;
    bundle.list.push_back(a1);
    bundle.list.push_back(a2);

    for(int i = 0; i < 15; i ++)
    {
        int j = 0;
        for(msAnimationIterator a = bundle.list.begin(); a != bundle.list.end(); j ++, a ++)
        {
            msFromToAnimationContext<GLfloat*> *context = (msFromToAnimationContext<GLfloat*>*) (*a)->getContext();
            printf("value%d: %f\r\n", j, *context->getFrom());            
        }

        bundle.performStep();
    }
}

GLint msAnimationBundle::getCount()
{
    return this->list.size();
}

void msAnimationBundle::add(msAnimation *animation)
{
    this->list.push_back(animation);
}

void msAnimationBundle::addAnimation(GLint delayCount, GLint count, msAnimationContext *context, void (*stepCallback)(msAnimationContext*))
{
    this->list.push_back(new msAnimation(delayCount, count, context, stepCallback));
}

void msAnimationBundle::addSerialAnimation(GLint count, msAnimationContext *context, void (*stepCallback)(msAnimationContext*))
{
    if((int)this->list.size() > 0)
    {
        msAnimation *last = this->list.back();
        this->list.push_back(new msAnimation(last->getDelayCount() + last->getCount(), count, context, stepCallback));        
    }
}

msAnimationIterator msAnimationBundle::getFirst()
{
    return this->list.begin();
}

msAnimationIterator msAnimationBundle::getLast()
{
    return this->list.end();
}