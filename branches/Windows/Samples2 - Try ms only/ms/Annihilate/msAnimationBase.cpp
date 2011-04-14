#include "msAnimationBase.h"
#include "msAnimation.h"

msAnimationBase::msAnimationBase(GLint delayCount, GLint count, void (*stepCallback)(msAnimationBase*))
{
	m_delayCount = delayCount;
	m_count = count;
	m_performStep = (stepCallback == 0) ? _defaultStep : stepCallback;
}

msAnimationBase::~msAnimationBase()
{

}

void msAnimationBase::_defaultStep(msAnimationBase *anim)
{
	msAnimation<float> *a = (msAnimation<float>*)anim;
	float step = (a->m_to - a->m_from) / a->m_count;
	a->m_from += step;
}

// NOTE:	don't forget to delete animation object after the last perform step has executed
GLint msAnimationBase::performStep()
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

