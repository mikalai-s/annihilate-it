#include "msBox.h"
#include "stdlib.h"
#include "stdio.h"
#include "msPalette.h"
#include "msBoxGrid.h"

msBox::msBox()
{
	_init(0, MS_BOX_INVISIBLE);	
}

msBox::msBox(float x, float y, float width, float height, int colorIndex)
{	
	_init(0, colorIndex);	
}

msBox::msBox(msBoxVertexData *verticesData, int colorIndex)
{
	_init(verticesData, colorIndex);	
}



void msBox::_init(msBoxVertexData *verticesData, int colorIndex )
{
    m_verticesData = verticesData;

	m_location = getVerticesData()->vertices;	
	
	m_colorIndex = colorIndex;
	m_originalColorIndex = colorIndex;
	m_border = (msBorder*)malloc(sizeof(msBorder));
	m_border->left = 0;
	m_border->top = 0;
	m_border->right = 0;
	m_border->bottom = 0;

	m_colorDisturbance.a = 1.0f;
	m_colorDisturbance.r = 1.0f;
	m_colorDisturbance.g = 1.0f;
	m_colorDisturbance.b = 1.0f;

	m_animations = new msAnimationBundle();

	m_requiresExplosion = false;
	m_requiresWave = false;

	m_top = 0;
	m_right = 0;
	m_bottom = 0;
	m_left = 0;

	m_angle = 0.0f;	
}

msBox::~msBox()
{
	free(m_border);

	delete m_animations;
}

void msBox::makeInvisible()
{
	m_colorIndex = MS_BOX_INVISIBLE;
}


GLboolean msBox::isVisible()
{
	if(this == 0)
		return GL_FALSE;

	return (m_colorIndex != MS_BOX_INVISIBLE);
}

msAnimationBundle* msBox::getAnimations()
{
    return m_animations;
}

void msBox::unitTest()
{
	msBox *box = new msBox(0, 0, 100, 100, 2);
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
	msKeyValueAnimationContext<msBoxVertexData*, msPointf> *context = (msKeyValueAnimationContext<msBoxVertexData*, msPointf> *)c;

	msBoxVertexData *vertexData = context->getKey();
	msPointf to = context->getValue();

    float dx = (to.x - vertexData->vertices[0].x) / context->getAnimation()->getCount();
    float dy = (to.y - vertexData->vertices[0].y) / context->getAnimation()->getCount();

    vertexData->move(msPointf(dx, dy, 0));
}

void msBox::_finishLinearFalling(msAnimationContext *c)
{
    msKeyValueAnimationContext<msBoxVertexData*, msBoxVertexData*> *context = (msKeyValueAnimationContext<msBoxVertexData*, msBoxVertexData*> *)c;

    msBoxVertexData *fromVertexData = context->getKey();
    msBoxVertexData *toVertexData = context->getValue();

    fromVertexData->copy(toVertexData);
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


void msBox::fall(GLint delay, GLint direction, msBoxVertexData *newVertexData)
{
    int times = 10;

    // moving from top to bottom
    msKeyValueAnimationContext<msBoxVertexData*, msPointf> *c2 = new msKeyValueAnimationContext<msBoxVertexData*, msPointf>(m_verticesData, newVertexData->vertices[0]);
	getAnimations()->add(new msAnimation(delay, times, c2, _linearFalling));
    
    // move a little bit up for effect of bounce
    msPointf bouncePoint(newVertexData->vertices[0].x, newVertexData->vertices[0].y - MS_BOUNCE_OFFSET * 4, newVertexData->vertices[0].z);
    msKeyValueAnimationContext<msBoxVertexData*, msPointf> *c3 = new msKeyValueAnimationContext<msBoxVertexData*, msPointf>(m_verticesData, bouncePoint);
    getAnimations()->add(new msAnimation(delay + times, 4, c3, _linearFalling));
    
    // final falling (very fast and accurate)
    msKeyValueAnimationContext<msBoxVertexData*, msBoxVertexData*> *c4 = new msKeyValueAnimationContext<msBoxVertexData*, msBoxVertexData*>(m_verticesData, newVertexData);
    getAnimations()->add(new msAnimation(delay + times + 4, 1, c4, _finishLinearFalling));
}

void msBox::unfall( int delay, int direction, msBoxVertexData *newVertexData )
{
	int times = 10;

	// moving from top to bottom
	msKeyValueAnimationContext<msPointf*, msPointf> *c2 = new msKeyValueAnimationContext<msPointf*, msPointf>(m_location, newVertexData->vertices[0]);
	getAnimations()->add(new msAnimation(delay, times, c2, _linearFalling));

    // final falling (very fast and accurate)
    msKeyValueAnimationContext<msBoxVertexData*, msBoxVertexData*> *c4 = new msKeyValueAnimationContext<msBoxVertexData*, msBoxVertexData*>(m_verticesData, newVertexData);
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

	_setFlag<int>(delay + 2, &m_colorIndex, MS_BOX_INVISIBLE);
}

void rotate(msAnimationContext *c)
{
	msValueAnimationContext<float*> *con = (msValueAnimationContext<float*> *)c;
	float *angle = con->getValue();
	*angle -= 9;
}

void msBox::show( int delay )
{
	// restore original color after some delay
	m_colorIndex = m_originalColorIndex;

	// make 
	m_colorDisturbance.a = 0.0;
	
	// moving from top to bottom
	msKeyValueAnimationContext<float*, float> *c = new msKeyValueAnimationContext<float*, float>(&m_colorDisturbance.a, 1.0);
	getAnimations()->add(new msAnimation(delay, 15, c, _appearing));	

	m_angle = 90;

	msValueAnimationContext<float*> *c2 = new msValueAnimationContext<float*>(&m_angle);
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





void msBoxVertexData::copy( msBoxVertexData *source )
{
    memcpy(this, source, sizeof(msBoxVertexData));
}
