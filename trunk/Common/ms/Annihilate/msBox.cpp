#include "msBox.h"
#include "stdlib.h"
#include "stdio.h"
#include "msPalette.h"
#include "msBoxAnimation.h"

msBox::msBox()
{
	m_boxToAnimate = 0;

    m_colorDisturbance.a = 1.0;
    m_colorDisturbance.r = 1.0;
    m_colorDisturbance.g = 1.0;
    m_colorDisturbance.b = 1.0;
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
    m_border->left = 1;
    m_border->top = 1;
    m_border->right = 1;
    m_border->bottom = 1;

    m_colorDisturbance.a = 1.0f;
    m_colorDisturbance.r = 1.0f;
    m_colorDisturbance.g = 1.0f;
    m_colorDisturbance.b = 1.0f;
    
	m_boxToAnimate = new msBoxAnimation(this);
	m_boxToAnimate->m_location = m_location;
	m_boxToAnimate->m_size = m_size;
	m_boxToAnimate->m_colorIndex = m_colorIndex;
    m_boxToAnimate->m_border = m_border;
}


msBox::~msBox()
{
    if(m_boxToAnimate == 0)
        free(m_border);

	if(m_boxToAnimate != 0)
		delete m_boxToAnimate;
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

msBoxAnimation* msBox::getAnimated()
{
    return m_boxToAnimate;
}

void msBox::unitTest()
{
	msBox *box = new msBox(0, 0, 100, 100, 2);
	msAnimationBundle *anims = box->getAnimated()->getAnimations();

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