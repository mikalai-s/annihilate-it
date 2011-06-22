#pragma once

#include "msAnimationBundle.h"
#include "msAnimation.h"

#define MS_BOX_INVISIBLE -1


class msBox
{
	friend class  msBoxGrid;

private:

	msPointf m_location;
	msSize m_size;
	int m_colorIndex;
	int m_originalColorIndex;

	msColor m_colorDisturbance;

    // visibility of border
	msBorder *m_border;

	// animations related
	msAnimationBundle *m_animations;

	GLboolean m_requiresExplosion;
	msPointf m_explosionPoint;

    GLboolean m_requiresWave;

private:
	// for figure structure
	msBox *m_top;
	msBox *m_right;
	msBox *m_bottom;
	msBox *m_left;

public:
	float m_angle;
	


private:

	static void _linearFalling(msAnimationContext *c);

	static void _linearFalling2(msAnimationContext *c);

    static void _hiding1(msAnimationContext *c);

	static void _appearing(msAnimationContext *c);

    

	template <class T>
	static void _setFlagCallback(msAnimationContext *c)
	{
		msKeyValueAnimationContext<T, GLfloat> *context = (msKeyValueAnimationContext<T, GLfloat>*)c;
		*(context->getKey()) = context->getValue();
	}

    

	template <class T>
	void _setFlag(GLint delay, T *flag, T value)
	{
		msKeyValueAnimationContext<T*, GLfloat> *context = new msKeyValueAnimationContext<T*, GLfloat>(flag, value);
		getAnimations()->add(new msAnimation(delay, 1, context, _setFlagCallback<T*>));
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

	void fall(GLint delay, GLint direction, msPointf newLocation);

    void hide(int delay);

	void show( int delay );

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

	bool hasLeft()
	{
		return m_left != 0;
	}

	bool hasTop()
	{
		return m_top != 0;
	}

	bool hasRight()
	{
		return m_right != 0;
	}

	bool hasBottom()
	{
		return m_bottom != 0;
	}

	msPointf getLocation() const
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

	msColor getColorDisturbance() const
	{
		return m_colorDisturbance;
	}
	
	msPointf getExplosionPoint() const
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
	void unfall( int delay, int  direction, msPointf location );

};