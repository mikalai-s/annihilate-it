/*
 *  Common.h
 *  Tutorial1
 *
 *  Created by Michael Daley on 19/04/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "msGL.h"
#include "math.h"
#include <String>

using namespace std;



// Macro which returns a random value between -1 and 1
#define RANDOM_MINUS_1_TO_1() (((GLfloat)rand() / (GLfloat)RAND_MAX * 2.0f ) - 1.0f)

// MAcro which returns a random number between 0 and 1
#define RANDOM_0_TO_1() (((GLfloat)rand() / (GLfloat)RAND_MAX ))

#define GL_PI  (3.14159265f)

// Macro which converts degrees into radians
#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0f * GL_PI)

struct msColor
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;

    msColor()
    {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        a = 1.0f;
    }

    msColor(float rr, float gg, float bb, float aa)
    {
        r = rr;
        g = gg;
        b = bb;
        a = aa;
    }
};


struct msQuad
{
    GLfloat bl_x, bl_y;
    GLfloat br_x, br_y;
    GLfloat tl_x, tl_y;
    GLfloat tr_x, tr_y;
};


struct msPoint2f
{
    float x;
    float y;

    msPoint2f()
    {
        this->x = 0.f;
        this->y = 0.f;
    }

    msPoint2f(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void add(msPoint2f &p)
    {
        this->x += p.x;
        this->y += p.y;
    }

    void multiply(float d)
    {
        this->x *= d;
        this->y *= d;
    }
};


struct msPoint3f
{
    GLfloat x;
    GLfloat y;
    GLfloat z;

    msPoint3f()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    msPoint3f(float xx, float yy)
    {
        x = xx;
        y = yy;
        z = 0;
    }

    msPoint3f(float xx, float yy, float zz)
    {
        x = xx;
        y = yy;
        z = zz;
    }

	static float angle(msPoint3f p1, msPoint3f p2)
	{
		float d = (p1.x*p2.x + p1.y*p2.y + p1.z*p2.z);
		float l1 = sqrtf(p1.x*p1.x + p1.y*p1.y + p1.z*p1.z);
		float l2 = sqrtf(p2.x*p2.x + p2.y*p2.y + p2.z*p2.z);
		float cos = d / (l1 * l2);
		return acos(cos);
	}
};

struct msPointi
{
    int x;
    int y;
    int z;

    msPointi()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    msPointi(int xx, int yy)
    {
        x = xx;
        y = yy;
        z = 0;
    }
};



struct msSizef
{
    float width;
    float height;
};

struct msSizei
{
    int width;
    int height;
};


struct msBorder
{
    GLint left;
    GLint top;
    GLint right;
    GLint bottom;

    msColor color;
};

/*

static const msVector Vector2fZero = {0.0f, 0.0f};

static inline msVector Vector2fMake(GLfloat x, GLfloat y)
{
    msVector v;
    v.x = x;
    v.y = y;
    return v;
}

static inline msColor colorMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    msColor c;
    c.r = red;
    c.g = green;
    c.b = blue;
    c.a = alpha;
    return c;
}

static inline msVector Vector2fMultiply(msVector v, GLfloat s)
{
    msVector vv;
    vv.x = v.x * s;
    vv.y = v.y * s;
    return vv;
}

static inline msVector Vector2fAdd(msVector v1, msVector v2)
{
    msVector vr;
    vr.x = v1.x + v2.x;
    vr.y = v1.y + v2.y;
    return vr;
}

static inline msVector Vector2fSub(msVector v1, msVector v2)
{
    msVector vr;
    vr.x = v1.x - v2.x;
    vr.y = v1.y - v2.y;
    return vr;
}

static inline GLfloat Vector2fDot(msVector v1, msVector v2)
{
    return (GLfloat) v1.x * v2.x + v1.y * v2.y;
}

static inline GLfloat Vector2fLength(msVector v)
{
    return (GLfloat) sqrt(Vector2fDot(v, v));
}

static inline msVector Vector2fNormalize(msVector v)
{
    return Vector2fMultiply(v, 1.0f/Vector2fLength(v));
} 
*/
static inline char* copyString(string src)
{
    char *dest = new char [src.length() + 1];
    memcpy( dest, src.c_str(), src.length() + 1);
    return dest;
}

