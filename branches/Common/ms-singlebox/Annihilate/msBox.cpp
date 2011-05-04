#include "msBox.h"
#include "stdlib.h"
#include "stdio.h"
#include "msPalette.h"
#include "msBoxAnimation.h"
#include "msBoxGrid.h"

msBox::msBox()
{
//	m_boxToAnimate = 0;

    m_colorDisturbance.a = 1.0;
    m_colorDisturbance.r = 1.0;
    m_colorDisturbance.g = 1.0;
    m_colorDisturbance.b = 1.0;

	m_animations = new msAnimationBundle();

	m_requiresExplosion = false;
    m_requiresWaveInit = false;
    m_requiresWave = false;

}

msBox::msBox(float x, float y, float width, float height, int colorIndex)
{	
	m_location.x = x;
	m_location.y = y;
    m_location.z = 0.0f;
	m_size.width = width;
	m_size.height = height;
	m_colorIndex = colorIndex;
    m_border = (msBorder*)malloc(sizeof(msBorder));
    m_border->left = 0;
    m_border->top = 0;
    m_border->right = 0;
    m_border->bottom = 0;

    m_colorDisturbance.a = 1.0f;
    m_colorDisturbance.r = 1.0f;
    m_colorDisturbance.g = 1.0f;
    m_colorDisturbance.b = 1.0f;

	m_animations = new msAnimationBundle();

	m_requiresExplosion = false;
    m_requiresWaveInit = false;
    m_requiresWave = false;
    
	//m_boxToAnimate = new msBoxAnimation(this);
	//m_boxToAnimate->m_location = m_location;
	//m_boxToAnimate->m_size = m_size;
	//m_boxToAnimate->m_colorIndex = m_colorIndex;
 //   m_boxToAnimate->m_border = m_border;
}


msBox::~msBox()
{
//    if(m_boxToAnimate == 0)
        free(m_border);

	/*if(m_boxToAnimate != 0)
		delete m_boxToAnimate;*/

	delete m_animations;
}

msBox* msBox::copy(msBox *target)
{
    target->m_location = m_location;
    target->m_size = m_size;
    target->m_border = m_border;
	return target;
}

void msBox::makeInvisible()
{
	m_colorIndex = MS_BOX_INVISIBLE;
}


GLboolean msBox::isVisible()
{
	if(this == 0)
		return GL_FALSE;

	return (m_colorIndex != MS_BOX_INVISIBLE);
}

//msBoxAnimation* msBox::getAnimated()
//{
//    return m_boxToAnimate;
//}

msAnimationBundle* msBox::getAnimations()
{
    return m_animations;
}

void msBox::unitTest()
{
	msBox *box = new msBox(0, 0, 100, 100, 2);
	msAnimationBundle *anims = box->getAnimations();

	anims->add(new msAnimation(0, 5, new msFromToAnimationContext<GLfloat>(0, 2.5), 0));
	anims->add(new msAnimation(0, 10, new msFromToAnimationContext<GLfloat>(1, 7), 0));

	for(GLuint i = 0; i < 100; i ++)
	{
		for(msAnimationIterator ai = anims->getFirst(); ai != anims->getLast(); ai ++)
		{
			msFromToAnimationContext<GLfloat> *c = (msFromToAnimationContext<GLfloat> *)(*ai)->getContext();
			
			printf("%f\t%f\r\n", c->getFrom(), c->getTo());			
		}

		anims->performStep();
	}

	delete box;
}




void msBox::_linearFalling(msAnimationContext *c)
{
	msFromToAnimationContext<msPoint*> *context = (msFromToAnimationContext<msPoint*> *)c;

	msPoint *from = context->getFrom();
	msPoint *to = context->getTo();
	
	from->x += (to->x - from->x) / context->getAnimation()->getCount();
	from->y += (to->y - from->y) / context->getAnimation()->getCount();
}


void msBox::_linearFalling2(msAnimationContext *c)
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


void msBox::fall(GLint delay, GLint direction)
{
    int times = 10;

    // move a little bit up - to look like explosion raise boxes
    msPointMoveAnimationContext *c1 = new msPointMoveAnimationContext(&m_location, direction);
	getAnimations()->add(new msAnimation(0, 4, c1, _linearFalling2));

    // moving from top to bottom
    msFromToAnimationContext<msPoint*> *c2 = new msFromToAnimationContext<msPoint*>(&m_location, &m_location);
	getAnimations()->add(new msAnimation(delay + 4, times, c2, _linearFalling));
    
    // move a litle bit up for effect of bounce
    msPointMoveAnimationContext *c3 = new msPointMoveAnimationContext(&m_location, direction);
    getAnimations()->add(new msAnimation(delay + times + 4, 4, c3, _linearFalling2));
    
    // final falling (very quick)
    msFromToAnimationContext<msPoint*> *c4 = new msFromToAnimationContext<msPoint*>(&m_location, &m_location);
    getAnimations()->add(new msAnimation(delay + times + 4 + 4, 1, c4, _linearFalling));
}


void msBox::_hiding1(msAnimationContext *c)
{
    msFromToAnimationContext<msBoxAnimation*> *context = (msFromToAnimationContext<msBoxAnimation*>*)c;

    GLfloat k = 1.0f / context->getAnimation()->getCount();

    //context->getFrom()->m_colorDisturbance.a = 0.5;// -= 0.01f;
    context->getFrom()->m_colorDisturbance.r -= k;
    context->getFrom()->m_colorDisturbance.g -= k;
    context->getFrom()->m_colorDisturbance.b -= k;
}







void msBox::hide(GLint delay)
{
    // set requires explosion to make renderer know about that
    m_requiresExplosion = false;

	// store coordinates of explosion
	m_explosionPoint.x = m_location.x + m_size.width / 2.0f;
	m_explosionPoint.y = m_location.y + m_size.height / 2.0f;

    setFlag<GLboolean>(delay + 1, &m_requiresExplosion, true);

    // the following animation will be called only once and it will just reset the requires explosion flag to stop its animation
    setFlag<GLboolean>(delay + 2, &m_requiresExplosion, false);

	setFlag<int>(delay + 2, &m_colorIndex, MS_BOX_INVISIBLE);
}


void msBox::wave(GLint delay)
{
    m_requiresWaveInit = false;
    m_requiresWave = false;

    setFlag<GLboolean>(5, &m_requiresWaveInit, true);
    setFlag<GLboolean>(6, &m_requiresWaveInit, false);

    setFlag<GLboolean>(7, &m_requiresWave, true);
    setFlag<GLboolean>(70, &m_requiresWave, false);
}


GLboolean msBox::getRequiresExplosion()
{
    return m_requiresExplosion;
}

GLboolean msBox::getRequiresWave()
{
    return m_requiresWave;
}

GLboolean msBox::getRequiresWaveInit()
{
    return m_requiresWaveInit;
}