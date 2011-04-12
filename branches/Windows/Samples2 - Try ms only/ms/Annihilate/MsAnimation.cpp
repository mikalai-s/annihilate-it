
#include "MsAnimation.h"
#include "stdlib.h"
#include "stdio.h"

MsAnimationBase::MsAnimationBase(GLint delayCount, GLint count, void (*stepCallback)(MsAnimationBase*))
{
	m_delayCount = delayCount;
	m_count = count;
	m_performStep = (stepCallback == 0) ? _defaultStep : stepCallback;
}

void MsAnimationBase::_defaultStep(MsAnimationBase *anim)
{
	MsAnimation<float> *a = (MsAnimation<float>*)anim;
	float step = (a->m_to - a->m_from) / a->m_count;
	a->m_from += step;
}

// NOTE:	don't forget to delete animation object after the last perform step has executed
GLint MsAnimationBase::performStep()
{
	if(m_performStep == 0)
		return 0;

	if(m_count <= 0)
		return 0;

	if(m_delayCount > 0)
	{
		m_delayCount --;
		return 1;
	}

	m_performStep(this);

	m_count --;

	return (m_count >= 0);
}



template <class T>
MsAnimation<T>::MsAnimation(T from, T to, GLint delayCount, GLint count, void (*stepCallback)(MsAnimationBase*))
{
	m_from = from;
	m_to = to;
	m_delayCount = delayCount;
	m_count = count;
	m_performStep = (stepCallback == 0) ? _defaultStep : stepCallback;

    
    m_rAngle = 0.0f;
    m_rVector.x = 0.0f;
    m_rVector.y = 0.0f;
    m_rVector.z = 0.0f;
}



template <class T>
MsAnimation<T>::~MsAnimation()
{
}

template <class T>
void MsAnimation<T>::_lineStep(MsAnimationBase *ab)
{
	MsAnimation<point*> *anim = (MsAnimation<point*>*)ab;
	point *from = ab->m_from;
	point *to = ab->m_to;
	point step;
	step.x = (to->x - from->x) / m_count;
	step.y = (to->y - from->y) / m_count;

	from->x += step.x;
	from->y += step.y;
}

template <class T>
void MsAnimation<T>::unitTest()
{
	MsAnimation<point*>	*anim;

	point from;
	point to;
	int count = 10;

	from.x = 0;
	from.y = 0;

	to.x = 10;
	to.y = 5;

	anim = new MsAnimation(&from, &to, 0, count, _lineStep);

	do
	{
		printf("(%f, %f)\r\n", ((point*)anim->m_from)->x, ((point*)anim->m_from)->y);
	}
	while(anim->performStep(anim));

	delete anim;
}
