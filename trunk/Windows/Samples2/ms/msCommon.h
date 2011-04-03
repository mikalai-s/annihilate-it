/*
 *  Common.h
 *  Tutorial1
 *
 *  Created by Michael Daley on 19/04/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include <GLES2/gl2.h>
#include "math.h"





// Macro which returns a random value between -1 and 1
#define RANDOM_MINUS_1_TO_1() (((GLfloat)rand() / (GLfloat)RAND_MAX * 2.0f ) - 1.0f)

// MAcro which returns a random number between 0 and 1
#define RANDOM_0_TO_1() (((GLfloat)rand() / (GLfloat)RAND_MAX ))

#define GL_PI  (3.14159265f)

// Macro which converts degrees into radians
#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0f * GL_PI)


typedef struct _Color4f {
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;
} Color4f;

typedef struct _Vector2f {
	GLfloat x;
	GLfloat y;
} Vector2f;

typedef struct _Quad2f {
	GLfloat bl_x, bl_y;
	GLfloat br_x, br_y;
	GLfloat tl_x, tl_y;
	GLfloat tr_x, tr_y;
} Quad2f;

typedef struct _Particle {
	Vector2f position;
	Vector2f direction;
	Color4f color;
	Color4f deltaColor;
	GLfloat particleSize;
	GLfloat timeToLive;
} Particle;


typedef struct _PointSprite {
	GLfloat x;
	GLfloat y;
	GLfloat size;
} PointSprite;



static const Vector2f Vector2fZero = {0.0f, 0.0f};

static inline Vector2f Vector2fMake(GLfloat x, GLfloat y)
{
	Vector2f v;
	v.x = x;
	v.y = y;
	return v;
}

static inline Color4f Color4fMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	Color4f c;
	c.red = red;
	c.green = green;
	c.blue = blue;
	c.alpha = alpha;
	return c;
}

static inline Vector2f Vector2fMultiply(Vector2f v, GLfloat s)
{
	Vector2f vv;
	vv.x = v.x * s;
	vv.y = v.y * s;
	return vv;
}

static inline Vector2f Vector2fAdd(Vector2f v1, Vector2f v2)
{
	Vector2f vr;
	vr.x = v1.x + v2.x;
	vr.y = v1.y + v2.y;
	return vr;
}

static inline Vector2f Vector2fSub(Vector2f v1, Vector2f v2)
{
	Vector2f vr;
	vr.x = v1.x - v2.x;
	vr.y = v1.y - v2.y;
	return vr;
}

static inline GLfloat Vector2fDot(Vector2f v1, Vector2f v2)
{
	return (GLfloat) v1.x * v2.x + v1.y * v2.y;
}

static inline GLfloat Vector2fLength(Vector2f v)
{
	return (GLfloat) sqrt(Vector2fDot(v, v));
}

static inline Vector2f Vector2fNormalize(Vector2f v)
{
	return Vector2fMultiply(v, 1.0f/Vector2fLength(v));
} 