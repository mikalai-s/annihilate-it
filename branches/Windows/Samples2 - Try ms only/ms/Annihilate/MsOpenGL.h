/*
 *  MsOpenGL.h
 *  AnnihilateIt
 *
 *  Created by user on 11-03-05.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <OpenGLES/ES1/gl.h>
#include "MsCommon.h"
#include "MsBox.h"
#include "MsBoxGrid.h"
#include "MsPalette.h"

void ms_gl_draw_box(MsPalette *palette, MsBox *box, color *color);

void ms_gl_draw_left_border(MsBox *box, color *color);
void ms_gl_draw_top_border(MsBox *box, color *color);
void ms_gl_draw_right_border(MsBox *box, color *color);
void ms_gl_draw_bottom_border(MsBox *box, color *color);

void ms_gl_draw_boxgrid(MsBoxGrid *boxGrid);