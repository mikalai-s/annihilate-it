#pragma once

#include "msCommon.h"

class MsAnimationBase
{
public:
	GLint m_delayCount;
	GLint m_count;

	static void _defaultStep(MsAnimationBase *anim);

public:

	void (*m_performStep)(MsAnimationBase*);

	MsAnimationBase(GLint delayCount, GLint count, void (*stepCallback)(MsAnimationBase*));

	GLint performStep();
};



template <class T>
class MsAnimation : public MsAnimationBase
{
public:
	T m_from;
	T m_to;

	GLfloat m_rAngle;
	point m_rVector;

	static void _lineStep(MsAnimationBase *ab)
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

	MsAnimation(T from, T to, GLint delayCount, GLint count, void (*stepCallback)(MsAnimationBase*)) : MsAnimationBase(delayCount, count, stepCallback)
	{
		m_from = from;
		m_to = to;

		m_rAngle = 0.0f;
		m_rVector.x = 0.0f;
		m_rVector.y = 0.0f;
		m_rVector.z = 0.0f;
	}

	~MsAnimation()
	{

	}
/*
	static void unitTest();
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
	*/
};


