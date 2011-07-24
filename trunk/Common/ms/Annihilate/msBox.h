#pragma once

#include "msAnimationBundle.h"
#include "msAnimation.h"

struct msBoxFaceData
{
public:
    // color information
    int colorIndex;

    // color disturbance
    msColor colorDisturbance;

    // borders
    int hasBorder[4]; // left, top, right, bottom
    int hasCornerBorder[4]; // LeftTop, TopRight, RightBottom, BottomLeft

public:

    int *getHasBorder() const
    {
        return (int*)hasBorder;
    }

    int *getHasCornerBorder() const
    {
        return (int*)hasCornerBorder;
    }

    bool hasLeft()
    {
        return !hasBorder[0];
    }

    bool hasTop()
    {
        return !hasBorder[1];
    }

    bool hasRight()
    {
        return !hasBorder[2];
    }

    bool hasBottom()
    {
        return !hasBorder[3];
    }

    int getColorIndex() const 
    {
        return colorIndex;
    }

    msColor getColorDisturbance() const
    {
        return colorDisturbance;
    }
};

struct msBoxData
{
    // todo: make provate
public:
    // vertices
    msPoint3f vertices[4];    

    // rotation angle and vector
    float angle;
    msPoint3f angleVector;

    // front face
    msBoxFaceData frontFace;    

    // back face
    msBoxFaceData backFace;

public:
    msBoxData()
    {
    }

    bool isInside(msPoint3f *point)
    {
        return vertices[0].x <= point->x && vertices[1].x >= point->x &&
            vertices[0].y <= point->y && vertices[3].y >= point->y;
    }

    msPoint3f getCenter()
    {
        return msPoint3f((vertices[0].x + vertices[1].x) / 2.0f, (vertices[0].y + vertices[3].y) / 2.0f, (vertices[0].z + vertices[1].z) / 2.0f);
    }

    void copy( msBoxData *data );

    void exchange( msBoxData *source );

    void move(msPoint3f d)
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

    void copyVertices( msBoxData *source )
    {
        memcpy(this->vertices, source->vertices, sizeof(this->vertices));
    }
};


class msBox
{
    friend class  msBoxGrid;

private:

    msPoint3f *m_location;

    // animations related
    msAnimationBundle *m_animations;

    GLboolean m_requiresExplosion;
    msPoint3f m_explosionPoint;

    GLboolean m_requiresWave;

    bool m_visible;

private:

    msBoxData *m_verticesData;

private:

    static void _linearFalling(msAnimationContext *c);

    static void _finishLinearFalling(msAnimationContext *c);

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

    msBox(msBoxData *verticesData);

    ~msBox();

    void makeInvisible();

    GLboolean isVisible();

    msAnimationBundle *getAnimations();

    static void unitTest();

    void fall(GLint delay, GLint direction, msPoint3f *newVertices);

    void unfall( int delay, int  direction, msPoint3f *newVertices);

    void hide(int delay);

    void show( int delay );

    void wave(GLint delay);

    void _init(msBoxData *verticesData);

    
    int getColorIndex() const
    {
        return m_verticesData->frontFace.getColorIndex();
    }

    msColor getColorDisturbance() const
    {
        return m_verticesData->frontFace.getColorDisturbance();
    }
    

    msPoint3f *getLocation() const
    {
        return m_location;
    }    

    
    
    msPoint3f getExplosionPoint() const
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

    msBoxData *getVerticesData() const
    {
        return m_verticesData;
    }

    float getAngle()
    {
        return m_verticesData->angle;
    }

    msPoint3f getAngleVector()
    {
        return m_verticesData->angleVector;
    }
};