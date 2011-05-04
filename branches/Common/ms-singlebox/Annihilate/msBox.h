#pragma once

#include "msAnimationBundle.h"
#include "msAnimation.h"

#define MS_BOX_INVISIBLE -1


class msBox
{
public:
	msPoint m_location;
	msSize m_size;
	int m_colorIndex;
	msColor m_colorDisturbance;

    // visibility of border
	msBorder *m_border;

	// animations related
	msAnimationBundle *m_animations;

	GLboolean m_requiresExplosion;
	msPoint m_explosionPoint;

    GLboolean m_requiresWave;

    GLboolean m_requiresWaveInit;

private:
	// copy of the current box. is used just for animation. can be refactored in the future.
	//msBoxAnimation *m_boxToAnimate;	

	static void _linearFalling(msAnimationContext *c);

	static void _linearFalling2(msAnimationContext *c);

    static void _hiding1(msAnimationContext *c);

    

	template <class T>
	static void setFlagCallback(msAnimationContext *c)
	{
		msKeyValueAnimationContext<T, GLfloat> *context = (msKeyValueAnimationContext<T, GLfloat>*)c;
		*context->getKey() = context->getValue();
	}

    

	template <class T>
	void setFlag(GLint delay, T *flag, T value)
	{
		msKeyValueAnimationContext<T*, GLfloat> *context = new msKeyValueAnimationContext<T*, GLfloat>(flag, value);
		getAnimations()->add(new msAnimation(delay, 1, context, setFlagCallback<T*>));
	}

protected:
	msBox();

public:
	msBox(float x, float y, float width, float height, int colorIndex);

	~msBox();

	msBox* copy(msBox *target);

	void makeInvisible();

	GLboolean isVisible();

   // msBoxAnimation *getAnimated();

	msAnimationBundle *getAnimations();

	static void unitTest();

	void fall(GLint delay, GLint direction, msPoint newLocation);

    void hide(GLint delay);

    void wave(GLint delay);

    GLboolean getRequiresExplosion();

    GLboolean getRequiresWave();

    GLboolean getRequiresWaveInit();

};