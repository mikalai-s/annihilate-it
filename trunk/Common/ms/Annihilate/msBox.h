#pragma once

#include "msAnimationBundle.h"
#include "msAnimation.h"

#define MS_BOX_INVISIBLE -1


class msBox
{
	friend class  msBoxGrid;

private:

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

private:
	// for figure structure
	msBox *m_top;
	msBox *m_right;
	msBox *m_bottom;
	msBox *m_left;
	

private:

	static void _linearFalling(msAnimationContext *c);

	static void _linearFalling2(msAnimationContext *c);

    static void _hiding1(msAnimationContext *c);

    

	template <class T>
	static void setFlagCallback(msAnimationContext *c)
	{
		msKeyValueAnimationContext<T, GLfloat> *context = (msKeyValueAnimationContext<T, GLfloat>*)c;
		*(context->getKey()) = context->getValue();
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

	void makeInvisible();

	GLboolean isVisible();

	msAnimationBundle *getAnimations();

	static void unitTest();

	void fall(GLint delay, GLint direction, msPoint newLocation);

    void hide(GLint delay);

    void wave(GLint delay);

	void _init( float x, float y, float width, float height, int colorIndex );

	msBox *getTop() const
	{
		return m_top;
	}

	msBox *getRight() const
	{
		return m_right;
	}

	msBox *getBottom() const
	{
		return m_bottom;
	}

	msBox *getLeft() const
	{
		return m_left;
	}

	msPoint getLocation() const
	{
		return m_location;
	}

	msSize getSize() const
	{
		return m_size;
	}

	int getColorIndex() const 
	{
		return m_colorIndex;
	}	
	
	msPoint getExplosionPoint() const
	{
		return m_explosionPoint;
	}

	GLboolean getRequiresExplosion()
	{
		return m_requiresExplosion;
	}

	GLboolean getRequiresWave()
	{
		return m_requiresWave;
	}
};