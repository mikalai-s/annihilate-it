/*
 *  MsPalette.h
 *  AnnihilateIt
 *
 *  Created by user on 11-03-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "MsCommon.h"

#define MS_PALETTE_MAX_COLORS 100

#define MS_PALETTE_EMPTY_COLOR_R 0.0f
#define MS_PALETTE_EMPTY_COLOR_G 0.0f
#define MS_PALETTE_EMPTY_COLOR_B 0.0f
#define MS_PALETTE_EMPTY_COLOR_A 0.0f


class msPalette
{
	msColor m_colors[MS_PALETTE_MAX_COLORS];

public:
	msPalette();
	~msPalette();

	void init(GLfloat[][4], GLuint numColors);	

	void setColor(GLuint index, msColor *color);

	msColor* getColor(GLuint index);

	GLuint getFreeIndex();

	void setFreeIndex(GLuint index);

	msColor getEmptyColor();
};


