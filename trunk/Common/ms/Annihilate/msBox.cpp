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
    m_verticesData = verticesData;

	m_location = getVerticesData()->vertices;	
	
    m_animations = new msAnimationBundle();

	m_requiresExplosion = false;
	m_requiresWave = false;

	m_visible = true;
}

msBox::~msBox()
{
	delete m_animations;
}

void msBox::makeInvisible()
{
	m_visible = false;
}


GLboolean msBox::isVisible()
{
	if(this == 0)
		return false;

	return m_visible;
}

msAnimationBundle* msBox::getAnimations()
{
    return m_animations;
}

void msBox::unitTest()
{
	msBox *box = new msBox(0, 0, 100, 100);
	msAnimationBundle *anims = box->getAnimations();

	anims->add(new msAnimation(0, 5, new msFromToAnimationContext<GLfloat>(0, 2.5), 0));
	anims->add(new msAnimation(0, 10, new msFromToAnimationContext<GLfloat>(1, 7), 0));

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



void msBox::_linearFalling(msAnimationContext *c)
{
	msKeyValueAnimationContext<msBoxData*, msPointf> *context = (msKeyValueAnimationContext<msBoxData*, msPointf> *)c;

	msBoxData *vertexData = context->getKey();
	msPointf to = context->getValue();

    float dx = (to.x - vertexData->vertices[0].x) / context->getAnimation()->getCount();
    float dy = (to.y - vertexData->vertices[0].y) / context->getAnimation()->getCount();

    vertexData->move(msPointf(dx, dy, 0));
}

void msBox::_finishLinearFalling(msAnimationContext *c)
{
    msKeyValueAnimationContext<msBoxData*, msPointf*> *context = (msKeyValueAnimationContext<msBoxData*, msPointf*> *)c;

    msBoxData *fromVertexData = context->getKey();
    msPointf *toVertices = context->getValue();

	memcpy(fromVertexData->vertices, toVertices, sizeof(fromVertexData->vertices));
}

void msBox::_linearFalling2(msAnimationContext *c)
{
	msPointMoveAnimationContext *context = (msPointMoveAnimationContext*)c;
	msPointf *p = context->getPoint();
    switch(context->getDirection())
    {
        case MS_BOX_SHIFT_TOP:
            p->y += MS_BOUNCE_OFFSET;
            break;
            
        case MS_BOX_SHIFT_LEFT:
            p->x += MS_BOUNCE_OFFSET;
            break;
            
        case MS_BOX_SHIFT_RIGHT:
            p->x -= MS_BOUNCE_OFFSET;
            break;
            
        default:
            p->y -= MS_BOUNCE_OFFSET;
    }    
}


void msBox::fall(GLint delay, GLint direction, msPointf *newVertices)
{
    int times = 10;

    // moving from top to bottom
    msKeyValueAnimationContext<msBoxData*, msPointf> *c2 = new msKeyValueAnimationContext<msBoxData*, msPointf>(m_verticesData, newVertices[0]);
	getAnimations()->add(new msAnimation(delay, times, c2, _linearFalling));
    
    // move a little bit up for effect of bounce
    msPointf bouncePoint(newVertices[0].x, newVertices[0].y - MS_BOUNCE_OFFSET * 4, newVertices[0].z);
    msKeyValueAnimationContext<msBoxData*, msPointf> *c3 = new msKeyValueAnimationContext<msBoxData*, msPointf>(m_verticesData, bouncePoint);
    getAnimations()->add(new msAnimation(delay + times, 4, c3, _linearFalling));
    
    // final falling (very fast and accurate)
    msKeyValueAnimationContext<msBoxData*, msPointf*> *c4 = new msKeyValueAnimationContext<msBoxData*, msPointf*>(m_verticesData, newVertices);
    getAnimations()->add(new msAnimation(delay + times + 4, 1, c4, _finishLinearFalling));
}

void msBox::unfall( int delay, int direction,  msPointf *newVertices)
{
	int times = 10;

	// moving from top to bottom
	msKeyValueAnimationContext<msPointf*, msPointf> *c2 = new msKeyValueAnimationContext<msPointf*, msPointf>(m_location, newVertices[0]);
	getAnimations()->add(new msAnimation(delay, times, c2, _linearFalling));

    // final falling (very fast and accurate)
    msKeyValueAnimationContext<msBoxData*, msPointf*> *c4 = new msKeyValueAnimationContext<msBoxData*, msPointf*>(m_verticesData, newVertices);
    getAnimations()->add(new msAnimation(delay + times, 1, c4, _finishLinearFalling));
}



void msBox::hide(GLint delay)
{
    // set requires explosion to make renderer know about that
    m_requiresExplosion = false;

	// store coordinates of explosion
	m_explosionPoint = getVerticesData()->getCenter();

    _setFlag<GLboolean>(delay + 1, &m_requiresExplosion, true);

    // the following animation will be called only once and it will just reset the requires explosion flag to stop its animation
    _setFlag<GLboolean>(delay + 2, &m_requiresExplosion, false);

	_setFlag<bool>(delay + 2, &m_visible, false);
}

void rotate(msAnimationContext *c)
{
	msValueAnimationContext<float*> *con = (msValueAnimationContext<float*> *)c;
	float *angle = con->getValue();
	*angle -= 0.15707963 * 2.0f;
}

void msBox::show( int delay )
{
	m_visible = true;

	// make 
	m_verticesData->frontFace.colorDisturbance.a = 0.0;
	
	// moving from top to bottom
	msKeyValueAnimationContext<float*, float> *c = new msKeyValueAnimationContext<float*, float>(&m_verticesData->frontFace.colorDisturbance.a, 1.0);
	getAnimations()->add(new msAnimation(delay, 15, c, _appearing));	

	m_verticesData->angle = 90.0f * 3.1415926f / 180.0f * 2.0f;

	msValueAnimationContext<float*> *c2 = new msValueAnimationContext<float*>(&m_verticesData->angle);
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
    m_requiresWave = false;

    _setFlag<GLboolean>(delay, &m_requiresWave, true);
    _setFlag<GLboolean>(delay + 1, &m_requiresWave, false);
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
