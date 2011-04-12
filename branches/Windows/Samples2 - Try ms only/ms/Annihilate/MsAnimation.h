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

	static void _lineStep(MsAnimationBase *ab);

	MsAnimation(T from, T to, GLint delayCount, GLint count, void (*stepCallback)(MsAnimationBase*));

	~MsAnimation();

	static void unitTest();
};


