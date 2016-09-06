#include "msBox.h"
#include "stdlib.h"
#include "stdio.h"
#include "msPalette.h"
#include "msBoxGrid.h"

msBox::msBox()
{
    _init(0);    
}

msBox::msBox(float x, float y, float width, float height)
{    
    _init(0);    
}

msBox::msBox(msBoxData *verticesData)
{
    _init(verticesData);    
}



void msBox::_init(msBoxData *verticesData)
{
    this->verticesData = verticesData;

    this->location = getVerticesData()->vertices;    
    
    this->animations = new msAnimationBundle();

    this->requiresExplosion = false;
    this->requiresWave = false;

    this->visible = true;
    
    this->falling = false;
}

msBox::~msBox()
{
    delete this->animations;
}

void msBox::makeInvisible()
{
    this->visible = false;
}


GLboolean msBox::isVisible()
{
    if(this == 0)
        return false;

    return this->visible;
}

msAnimationBundle* msBox::getAnimations()
{
    return this->animations;
}

void msBox::unitTest()
{
    msBox *box = new msBox(0, 0, 100, 100);
    msAnimationBundle *anims = box->getAnimations();

    anims->add(new msAnimation(0, 5, new msFromToAnimationContext<GLfloat>(0, 0, 2.5), 0));
    anims->add(new msAnimation(0, 10, new msFromToAnimationContext<GLfloat>(0, 1, 7), 0));

    for(GLuint i = 0; i < 100; i ++)
    {
        for(msAnimationIterator ai = anims->getFirst(); ai != anims->getLast(); ai ++)
        {
            msFromToAnimationContext<GLfloat> *c = (msFromToAnimationContext<GLfloat> *)(*ai)->getContext();
            
            printf("%f\t%f\r\n", c->getFrom(), c->getTo());            
        }

        anims->performStep();
    }

    delete box;
}



void msBox::_linearFallingWithCheck(msAnimationContext *c)
{
    // stop animation when box falling stopped otherwise continue falling auimation with usual callback
    msBox *owner = (msBox*)c->getOwner();
    if(owner->falling)
        _linearFalling(c);
    else
        c->getAnimation()->stop();
}

void msBox::_linearFalling(msAnimationContext *c)
{
    msKeyValueAnimationContext<msBoxData*, msPoint3f> *context = (msKeyValueAnimationContext<msBoxData*, msPoint3f> *)c;
    
    msBoxData *vertexData = context->getKey();
    msPoint3f to = context->getValue();
    
    float dx = (to.x - vertexData->vertices[0].x) / context->getAnimation()->getCount();
    float dy = (to.y - vertexData->vertices[0].y) / context->getAnimation()->getCount();
    
    vertexData->translate(msPoint3f(dx, dy, 0));
}

void msBox::_finishLinearFallingWithCheck(msAnimationContext *c)
{
    // stop animation when box falling stopped otherwise continue falling animation with usual callback
    msBox *owner = (msBox*)c->getOwner();
    if(owner->falling)
        _finishLinearFalling(c);
    else
        c->getAnimation()->stop();
}

void msBox::_finishLinearFalling(msAnimationContext *c)
{
    msKeyValueAnimationContext<msBoxData*, msPoint3f*> *context = (msKeyValueAnimationContext<msBoxData*, msPoint3f*> *)c;

    msBoxData *fromVertexData = context->getKey();
    msPoint3f *toVertices = context->getValue();

    memcpy(fromVertexData->vertices, toVertices, sizeof(fromVertexData->vertices));
    
    msBox *owner = (msBox*)c->getOwner();
    owner->falling = false;
}


void msBox::fall(GLint delay, GLint direction, msPoint3f *newVertices)
{
    this->falling = true;

    // moving from top to bottom
    msKeyValueAnimationContext<msBoxData*, msPoint3f> *c2 = new msKeyValueAnimationContext<msBoxData*, msPoint3f>(this, this->verticesData, newVertices[0]);
    getAnimations()->addAnimation(delay, 10, c2, _linearFallingWithCheck);
    
    // move a little bit up for effect of bounce
    msPoint3f bouncePoint(newVertices[0].x, newVertices[0].y, newVertices[0].z);
    switch(direction)
    {
        case MS_BOX_SHIFT_TOP:
            bouncePoint.y += MS_BOUNCE_OFFSET;
            break;
            
        case MS_BOX_SHIFT_LEFT:
            bouncePoint.x += MS_BOUNCE_OFFSET;
            break;
            
        case MS_BOX_SHIFT_RIGHT:
            bouncePoint.x -= MS_BOUNCE_OFFSET;
            break;
            
        default:
            bouncePoint.y -= MS_BOUNCE_OFFSET;
    }    
    msKeyValueAnimationContext<msBoxData*, msPoint3f> *c3 = new msKeyValueAnimationContext<msBoxData*, msPoint3f>(this, this->verticesData, bouncePoint);
    getAnimations()->addSerialAnimation(4, c3, _linearFallingWithCheck);
    
    // final falling (very fast and accurate)
    msKeyValueAnimationContext<msBoxData*, msPoint3f*> *c4 = new msKeyValueAnimationContext<msBoxData*, msPoint3f*>(this, this->verticesData, newVertices);
    getAnimations()->addSerialAnimation(1, c4, _finishLinearFallingWithCheck);
}

void msBox::unfall( int delay, int direction,  msPoint3f *newVertices)
{
    // if box is currently falling and we need to stop it
    this->falling = false;
    
    int times = 10;

    // moving from top to bottom
    msKeyValueAnimationContext<msPoint3f*, msPoint3f> *c2 = new msKeyValueAnimationContext<msPoint3f*, msPoint3f>(this, this->location, newVertices[0]);
    getAnimations()->add(new msAnimation(delay, times, c2, _linearFalling));

    // final falling (very fast and accurate)
    msKeyValueAnimationContext<msBoxData*, msPoint3f*> *c4 = new msKeyValueAnimationContext<msBoxData*, msPoint3f*>(this, this->verticesData, newVertices);
    getAnimations()->add(new msAnimation(delay + times, 1, c4, _finishLinearFalling));
}



void msBox::hide(GLint delay)
{
    // set requires explosion to make renderer know about that
    this->requiresExplosion = false;

    // store coordinates of explosion
    this->explosionPoint = getVerticesData()->getCenter();
    
    getAnimations()->setValueLazily<GLboolean>(delay + 1, &this->requiresExplosion, true);

    // the following animation will be called only once and it will just reset the requires explosion flag to stop its animation
    getAnimations()->setValueLazily<GLboolean>(delay + 2, &this->requiresExplosion, false);

    getAnimations()->setValueLazily<bool>(delay + 2, &this->visible, false);
}

void rotate(msAnimationContext *c)
{
    msValueAnimationContext<float*> *con = (msValueAnimationContext<float*> *)c;
    float *angle = con->getValue();
    *angle -= 0.15707963 * 2.0f;
}

void msBox::show( int delay )
{
    this->visible = true;

    // make 
    this->verticesData->frontFace.colorDisturbance.a = 0.0;
    
    // moving from top to bottom
    msKeyValueAnimationContext<float*, float> *c = new msKeyValueAnimationContext<float*, float>(this, &this->verticesData->frontFace.colorDisturbance.a, 1.0);
    getAnimations()->add(new msAnimation(delay, 15, c, _appearing));    

    this->verticesData->angle = 90.0f * 3.1415926f / 180.0f * 2.0f;

    msValueAnimationContext<float*> *c2 = new msValueAnimationContext<float*>(this, &this->verticesData->angle);
    msAnimation *a = new msAnimation(0, 10, c2, rotate);
    getAnimations()->add(a);
}

void msBox::_appearing(msAnimationContext *c)
{
    msKeyValueAnimationContext<float*, float> *context = (msKeyValueAnimationContext<float*, float> *)c;

    float *from = context->getKey();
    float to = context->getValue();

    *from += (to - *from) / context->getAnimation()->getCount();
}

void msBox::wave(GLint delay)
{
    this->requiresWave = false;

    getAnimations()->setValueLazily<GLboolean>(delay, &this->requiresWave, true);
    getAnimations()->setValueLazily<GLboolean>(delay + 1, &this->requiresWave, false);
}

void msBoxData::copy( msBoxData *source )
{
    memcpy(this, source, sizeof(msBoxData));
}

void msBoxData::exchange( msBoxData *source )
{
    msBoxData temp;
    memcpy(&temp, source, sizeof(msBoxData));
    memcpy(source, this, sizeof(msBoxData));
    memcpy(this, &temp, sizeof(msBoxData));
}
