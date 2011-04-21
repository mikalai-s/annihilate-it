#include "msBoxAnimation.h"
#include "msBoxGrid.h"


msBoxAnimation::msBoxAnimation(msBox *box)
{
    m_animations = new msAnimationBundle();
    m_box = box;
    m_requiresExplosion = false;
}


msBoxAnimation::~msBoxAnimation()
{
    delete m_animations;
}


msAnimationBundle* msBoxAnimation::getAnimations()
{
    return m_animations;
}


void msBoxAnimation::_linearFalling(msAnimationContext *c)
{
	msFromToAnimationContext<msPoint*> *context = (msFromToAnimationContext<msPoint*> *)c;

	msPoint *from = context->getFrom();
	msPoint *to = context->getTo();
	
	from->x += (to->x - from->x) / context->getAnimation()->getCount();
	from->y += (to->y - from->y) / context->getAnimation()->getCount();
}


void msBoxAnimation::_linearFalling2(msAnimationContext *c)
{
	msPointMoveAnimationContext *context = (msPointMoveAnimationContext*)c;
	msPoint *p = context->getPoint();
    switch(context->getDirection())
    {
        case MS_BOX_SHIFT_TOP:
            p->y += MS_BOUNCE_OFFSET;
            break;
            
        case MS_BOX_SHIFT_LEFT:
            p->x += MS_BOUNCE_OFFSET;
            break;
            
        case MS_BOX_SHIFT_RIGHT:
            p->x -= MS_BOUNCE_OFFSET;
            break;
            
        default:
            p->y -= MS_BOUNCE_OFFSET;
    }    
}


void msBoxAnimation::fall(GLint delay, GLint direction)
{
    int times = 10;

    // moving from one position to another
    msFromToAnimationContext<msPoint*> *c1 = new msFromToAnimationContext<msPoint*>(&m_location, &m_box->m_location);
	getAnimations()->add(new msAnimation(delay, times, c1, _linearFalling));
    
    msPointMoveAnimationContext *c2 = new msPointMoveAnimationContext(&m_location, direction);
    getAnimations()->add(new msAnimation(delay + times, 4, c2, _linearFalling2));
    
    msFromToAnimationContext<msPoint*> *c3 = new msFromToAnimationContext<msPoint*>(&m_location, &m_box->m_location);
    getAnimations()->add(new msAnimation(delay + times + 4, 1, c3, _linearFalling));
}


void msBoxAnimation::_hiding1(msAnimationContext *c)
{
    msFromToAnimationContext<msBoxAnimation*> *context = (msFromToAnimationContext<msBoxAnimation*>*)c;

    GLfloat k = 1.0f / context->getAnimation()->getCount();

    //context->getFrom()->m_colorDisturbance.a = 0.5;// -= 0.01f;
    context->getFrom()->m_colorDisturbance.r -= k;
    context->getFrom()->m_colorDisturbance.g -= k;
    context->getFrom()->m_colorDisturbance.b -= k;
}

void msBoxAnimation::_resetRequiresAnimation(msAnimationContext *c)
{
    msValueAnimationContext<msBoxAnimation*> *context = (msValueAnimationContext<msBoxAnimation*>*)c;
    context->getValue()->m_requiresExplosion = false;
}


void msBoxAnimation::hide(GLint delay)
{
    // add animation
    //msFromToAnimationContext<msBoxAnimation*> *context = new msFromToAnimationContext<msBoxAnimation*>(this, 0);
    //getAnimations()->add(new msAnimation(delay, 50, context, _hiding1));


    // set requires explosion to make renderer know about that
    m_requiresExplosion = true;

    // the following animation will be called only once and it will just reset the requires explosion flag to stop its animation
    GLint duration = 10;
    msValueAnimationContext<msBoxAnimation*> *context = new msValueAnimationContext<msBoxAnimation*>(this);
    getAnimations()->add(new msAnimation(duration, 1, context, _resetRequiresAnimation));
}


GLboolean msBoxAnimation::getRequiresExplosion()
{
    return m_requiresExplosion;
}