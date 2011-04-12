#pragma once

#include "MsCommon.h"

#include "MsAnimation.h"

class MsAnimation;

class MsAnimationBase
{
public:
	GLint m_delayCount;
	GLint m_count;

	static void _defaultStep(MsAnimationBase *anim)
	{
		MsAnimation<float> *a = (MsAnimation<float>*)anim;
		float step = (a->m_to - a->m_from) / a->m_count;
		a->m_from += step;
	}

public:

	void (*m_performStep)(MsAnimationBase*);



	MsAnimationBase(GLint delayCount, GLint count, void (*stepCallback)(MsAnimationBase*))
	{
		m_delayCount = delayCount;
		m_count = count;
		m_performStep = (stepCallback == 0) ? _defaultStep : stepCallback;
	}

	GLint performStep()
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
};