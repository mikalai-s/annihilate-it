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
	for(msAnimationIterator i = m_list.begin(); i != m_list.end(); i ++)
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
		m_list.remove(*i);
		delete (*i);
	}
}

msAnimationBundle::~msAnimationBundle()
{
	while(m_list.size() > 0)
	{
		msAnimation *a = m_list.front();
		m_list.remove(a);
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

	msAnimation *a1 = new msAnimation(0, times1, new msFromToAnimationContext<GLfloat*>(&from1, &to1), _lineStep2);
    msAnimation *a2 = new msAnimation(0, times2, new msFromToAnimationContext<GLfloat*>(&from2, &to2), _lineStep2);

	msAnimationBundle bundle;
	bundle.m_list.push_back(a1);
	bundle.m_list.push_back(a2);

	for(int i = 0; i < 15; i ++)
	{
		int j = 0;
		for(msAnimationIterator a = bundle.m_list.begin(); a != bundle.m_list.end(); j ++, a ++)
		{
			msFromToAnimationContext<GLfloat*> *context = (msFromToAnimationContext<GLfloat*>*) (*a)->getContext();
			printf("value%d: %f\r\n", j, *context->getFrom());			
		}

		bundle.performStep();
	}
}

GLint msAnimationBundle::getCount()
{
    return m_list.size();
}

void msAnimationBundle::add(msAnimation *animation)
{
    m_list.push_back(animation);
}

msAnimationIterator msAnimationBundle::getFirst()
{
    return m_list.begin();
}

msAnimationIterator msAnimationBundle::getLast()
{
    return m_list.end();
}