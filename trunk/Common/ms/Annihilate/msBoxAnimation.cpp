#include "msBoxAnimation.h"
#include "msBoxGrid.h"


msBoxAnimation::msBoxAnimation(msBox *box)
{
    m_animations = new msAnimationBundle();
    m_box = box;
    m_requiresExplosion = false;
    m_requiresWaveInit = false;
    m_requiresWave = false;
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

    // move a little bit up - to look like explosion raise boxes
    msPointMoveAnimationContext *c1 = new msPointMoveAnimationContext(&m_location, direction);
	getAnimations()->add(new msAnimation(0, 4, c1, _linearFalling2));

    // moving from top to bottom
    msFromToAnimationContext<msPoint*> *c2 = new msFromToAnimationContext<msPoint*>(&m_location, &m_box->m_location);
	getAnimations()->add(new msAnimation(delay + 4, times, c2, _linearFalling));
    
    // move a litle bit up for effect of bounce
    msPointMoveAnimationContext *c3 = new msPointMoveAnimationContext(&m_location, direction);
    getAnimations()->add(new msAnimation(delay + times + 4, 4, c3, _linearFalling2));
    
    // final falling (very quick)
    msFromToAnimationContext<msPoint*> *c4 = new msFromToAnimationContext<msPoint*>(&m_location, &m_box->m_location);
    getAnimations()->add(new msAnimation(delay + times + 4 + 4, 1, c4, _linearFalling));
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


void msBoxAnimation::setFlag(GLint delay, GLboolean *flag, GLboolean value)
{
    msKeyValueAnimationContext<GLboolean*, GLfloat> *context = new msKeyValueAnimationContext<GLboolean*, GLfloat>(flag, value);
    getAnimations()->add(new msAnimation(delay, 1, context, setFlagCallback));
}


void msBoxAnimation::setFlagCallback(msAnimationContext *c)
{
    msKeyValueAnimationContext<GLboolean*, GLfloat> *context = (msKeyValueAnimationContext<GLboolean*, GLfloat>*)c;
    *context->getKey() = context->getValue();
}


void msBoxAnimation::hide(GLint delay)
{
    // set requires explosion to make renderer know about that
    m_requiresExplosion = true;

    // the following animation will be called only once and it will just reset the requires explosion flag to stop its animation
    setFlag(10, &m_requiresExplosion, false);
}


void msBoxAnimation::wave(GLint delay)
{
    m_requiresWaveInit = false;
    m_requiresWave = false;

    setFlag(10, &m_requiresWaveInit, true);
    setFlag(11, &m_requiresWaveInit, false);

    setFlag(12, &m_requiresWave, true);
    setFlag(50, &m_requiresWave, false);
}


GLboolean msBoxAnimation::getRequiresExplosion()
{
    return m_requiresExplosion;
}

GLboolean msBoxAnimation::getRequiresWave()
{
    return m_requiresWave;
}

GLboolean msBoxAnimation::getRequiresWaveInit()
{
    return m_requiresWaveInit;
}