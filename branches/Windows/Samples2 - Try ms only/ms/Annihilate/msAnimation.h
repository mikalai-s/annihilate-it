#pragma once

#include "../msCommon.h"
#include "msAnimationBase.h"


template <class FROM_T, class TO_T = FROM_T>
class msAnimation : public msAnimationBase
{
public:
	FROM_T m_from;
	TO_T m_to;

	GLfloat m_rAngle;
	msPoint m_rVector;

	static void _lineStep(msAnimationBase *ab)
	{
		msAnimation<msPoint*> *anim = (msAnimation<msPoint*>*)ab;
		msPoint *from = anim->m_from;
		msPoint *to = anim->m_to;
		msPoint step;
		step.x = (to->x - from->x) / anim->m_count;
		step.y = (to->y - from->y) / anim->m_count;

		from->x += step.x;
		from->y += step.y;
	}

	msAnimation(FROM_T from, TO_T to, GLint delayCount, GLint count, void (*stepCallback)(msAnimationBase*)) : msAnimationBase(delayCount, count, stepCallback)
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

	static void unitTest()
	{
		msPoint from;
		msPoint to;
		int count = 10;

		from.x = 0;
		from.y = 0;

		to.x = 10;
		to.y = 5;

		msAnimation<msPoint*> *anim = new msAnimation(&from, &to, 0, count, _lineStep);

		do
		{
			printf("(%f, %f)\r\n", anim->m_from->x, anim->m_from->y);
		}
		while(anim->performStep());

		delete anim;
	}
};


