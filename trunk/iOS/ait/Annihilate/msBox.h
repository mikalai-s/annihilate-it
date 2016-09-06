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

    void translate(msPoint3f d)
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
    
    void move(msPoint3f p)
    {
        float width = vertices[1].x - vertices[0].x;
        float height = vertices[2].y - vertices[0].y;
        
        vertices[0] = p;
        
        vertices[1].x = p.x + width;
        vertices[1].y = p.y;
        
        vertices[2].x = p.x;
        vertices[2].y = p.y + height;
        
        vertices[3].x = p.x + width;
        vertices[3].y = p.y + height;
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

    msPoint3f *location;

    // animations related
    msAnimationBundle *animations;

    GLboolean requiresExplosion;
    msPoint3f explosionPoint;

    GLboolean requiresWave;
    
    bool falling;
    bool visible;

private:

    msBoxData *verticesData;

private:

    static void _linearFalling(msAnimationContext *c);
    
    static void _linearFallingWithCheck(msAnimationContext *c);

    static void _finishLinearFalling(msAnimationContext *c);
    
    static void _finishLinearFallingWithCheck(msAnimationContext *c);

    static void _appearing(msAnimationContext *c);

    
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
        return this->verticesData->frontFace.getColorIndex();
    }

    msColor getColorDisturbance() const
    {
        return this->verticesData->frontFace.getColorDisturbance();
    }
    

    msPoint3f *getLocation() const
    {
        return this->location;
    }    

    
    
    msPoint3f getExplosionPoint() const
    {
        return this->explosionPoint;
    }

    GLboolean getRequiresExplosion()
    {
        return this->requiresExplosion;
    }

    GLboolean getRequiresWave()
    {
        return this->requiresWave;
    }

    msBoxData *getVerticesData() const
    {
        return this->verticesData;
    }

    float getAngle()
    {
        return this->verticesData->angle;
    }

    msPoint3f getAngleVector()
    {
        return this->verticesData->angleVector;
    }
};