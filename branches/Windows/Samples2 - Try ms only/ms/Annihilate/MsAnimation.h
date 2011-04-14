#pragma once

#include "msCommon.h"
#include "msAnimationBase.h"


template <class T>
class msAnimation : public msAnimationBase
{
public:
	T m_from;
	T m_to;

	GLfloat m_rAngle;
	msPoint m_rVector;

	static void _lineStep(msAnimationBase *ab)
	{
		msAnimation<msPoint*> *anim = (msAnimation<msPoint*>*)ab;
		msPoint *from = ab->m_from;
		msPoint *to = ab->m_to;
		msPoint step;
		step.x = (to->x - from->x) / m_count;
		step.y = (to->y - from->y) / m_count;

		from->x += step.x;
		from->y += step.y;
	}

	msAnimation(T from, T to, GLint delayCount, GLint count, void (*stepCallback)(msAnimationBase*)) : msAnimationBase(delayCount, count, stepCallback)
	{
		m_from = from;
		m_to = to;

		m_rAngle = 0.0f;
		m_rVector.x = 0.0f;
		m_rVector.y = 0.0f;
		m_rVector.z = 0.0f;
	}

	~msAnimation()
	{

	}
/*
	static void unitTest();
	{
		MsAnimation<msPoint*>	*anim;

		msPoint from;
		msPoint to;
		int count = 10;

		from.x = 0;
		from.y = 0;

		to.x = 10;
		to.y = 5;

		anim = new MsAnimation(&from, &to, 0, count, _lineStep);

		do
		{
			printf("(%f, %f)\r\n", ((msPoint*)anim->m_from)->x, ((msPoint*)anim->m_from)->y);
		}
		while(anim->performStep(anim));

		delete anim;
	}
	*/
};


