#pragma once

#include "msAnimationBundle.h"
#include "msAnimation.h"

#define MS_BOX_INVISIBLE -1

struct msBoxVertexData
{
    msPointf vertices[4];

	int hasBorder[4]; // left, top, right, bottom
	int hasCornerBorder[4]; // LeftTop, TopRight, RightBottom, BottomLeft

    bool isInside(msPointf *point)
    {
        return vertices[0].x <= point->x && vertices[1].x >= point->x &&
            vertices[0].y <= point->y && vertices[3].y >= point->y;
    }

    msPointf getCenter()
    {
        return msPointf((vertices[0].x + vertices[1].x) / 2.0, (vertices[0].y + vertices[3].y) / 2.0, (vertices[0].z + vertices[1].z) / 2.0);
    }

    void copy( msBoxVertexData *data );

    void move(msPointf d)
    {
        float width = vertices[1].x - vertices[0].x;
        float height = vertices[2].y - vertices[0].y;

        vertices[0].x += d.x;
        vertices[0].y += d.y;
        
        vertices[1].x = vertices[0].x + width;
        vertices[1].y = vertices[0].y;
        
        vertices[2].x = vertices[0].x;
        vertices[2].y = vertices[0].y + height;
        
        vertices[3].x = vertices[0].x + width;
        vertices[3].y = vertices[0].y + height;
        
    }
};


class msBox
{
	friend class  msBoxGrid;

private:

	msPointf *m_location;
	int m_colorIndex;
	int m_originalColorIndex;

	int m_backColorIndex;
	int m_originalBackColorIndex;

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

	msBoxVertexData *m_verticesData;

	float m_angle;
    msPointf m_angleVector;
	


private:

	static void _linearFalling(msAnimationContext *c);

    static void _finishLinearFalling(msAnimationContext *c);

	static void _linearFalling2(msAnimationContext *c);


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

	msBox(msBoxVertexData *verticesData, int colorIndex);

	~msBox();

	void makeInvisible();

	GLboolean isVisible();

	msAnimationBundle *getAnimations();

	static void unitTest();

	void fall(GLint delay, GLint direction, msBoxVertexData *newVertexData);

    void unfall( int delay, int  direction, msBoxVertexData *newVertexData );

    void hide(int delay);

	void show( int delay );

    void wave(GLint delay);

	void _init(msBoxVertexData *verticesData, int colorIndex );

	bool hasLeft()
	{
		return m_verticesData->hasBorder[0];
	}

	bool hasTop()
	{
		return m_verticesData->hasBorder[1];
	}

	bool hasRight()
	{
		return m_verticesData->hasBorder[2];
	}

	bool hasBottom()
	{
		return m_verticesData->hasBorder[3];
	}

	msPointf *getLocation() const
	{
		return m_location;
	}	

	int getColorIndex() const 
	{
		return m_colorIndex;
	}

    int getBackColorIndex() const 
    {
        return m_backColorIndex;
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

    msBoxVertexData *getVerticesData() const
    {
        return m_verticesData;
    }

    float getAngle()
    {
        return m_angle;
    }

    msPointf getAngleVector()
    {
        return m_angleVector;
    }
};