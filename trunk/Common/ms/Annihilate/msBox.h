#pragma once

#include "msAnimationBundle.h"
#include "msAnimation.h"

struct msBoxVertexData
{
	// vertices
    msPointf vertices[4];

	// color information
	int colorIndex;

	// color disturbance
	msColor colorDisturbance;

	// rotation angle and vector
	float angle;
	msPointf angleVector;

	// borders
	int hasBorder[4]; // left, top, right, bottom
	int hasCornerBorder[4]; // LeftTop, TopRight, RightBottom, BottomLeft

	// TODO: remove. should be separate vertex data
	// back color information
	int backColorIndex;

	msBoxVertexData()
	{
	}

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

	void exchange( msBoxVertexData *source );

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

	void copyVertices( msBoxVertexData *source )
	{
		memcpy(this->vertices, source->vertices, sizeof(this->vertices));
	}
};


class msBox
{
	friend class  msBoxGrid;

private:

	msPointf *m_location;

	// animations related
	msAnimationBundle *m_animations;

	GLboolean m_requiresExplosion;
	msPointf m_explosionPoint;

    GLboolean m_requiresWave;

	bool m_visible;

private:

	msBoxVertexData *m_verticesData;
	


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
	msBox(float x, float y, float width, float height);

	msBox(msBoxVertexData *verticesData);

	~msBox();

	void makeInvisible();

	GLboolean isVisible();

	msAnimationBundle *getAnimations();

	static void unitTest();

	void fall(GLint delay, GLint direction, msPointf *newVertices);

    void unfall( int delay, int  direction, msPointf *newVertices);

    void hide(int delay);

	void show( int delay );

    void wave(GLint delay);

	void _init(msBoxVertexData *verticesData);

	

	bool hasLeft()
	{
		return !m_verticesData->hasBorder[0];
	}

	bool hasTop()
	{
		return !m_verticesData->hasBorder[1];
	}

	bool hasRight()
	{
		return !m_verticesData->hasBorder[2];
	}

	bool hasBottom()
	{
		return !m_verticesData->hasBorder[3];
	}

	msPointf *getLocation() const
	{
		return m_location;
	}	

	int getColorIndex() const 
	{
		return m_verticesData->colorIndex;
	}

    int getBackColorIndex() const 
    {
        return m_verticesData->backColorIndex;
    }    

	msColor getColorDisturbance() const
	{
		return m_verticesData->colorDisturbance;
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
        return m_verticesData->angle;
    }

    msPointf getAngleVector()
    {
        return m_verticesData->angleVector;
    }
};