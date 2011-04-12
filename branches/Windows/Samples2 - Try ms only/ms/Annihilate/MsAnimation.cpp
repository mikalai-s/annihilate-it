
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
