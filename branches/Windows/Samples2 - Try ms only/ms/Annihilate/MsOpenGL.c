/*
 *  MsOpenGL.c
 *  AnnihilateIt
 *
 *  Created by user on 11-03-05.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MsOpenGL.h"

GLfloat mBoxVertexesTemp[6][3]; // 6 vertexes for defining quad by means of two triangles (2 vertex for each)

float mBoxBordersTemp[6]; // array of vertexes for box borders

void ms_gl_draw_box(MsPalette *palette, MsBox *box, color *c)
{
    color  *boxColorTemp;
    
    msPoint l = box->location;
    msSize s = box->size;
    mBoxVertexesTemp[0][0] = l.x;               mBoxVertexesTemp[0][1] = l.y;               mBoxVertexesTemp[0][2] = l.z; 
    mBoxVertexesTemp[1][0] = l.x + s.width;     mBoxVertexesTemp[1][1] = l.y;               mBoxVertexesTemp[1][2] = l.z; 
    mBoxVertexesTemp[2][0] = l.x;               mBoxVertexesTemp[2][1] = l.y + s.height;    mBoxVertexesTemp[2][2] = l.z; 
    mBoxVertexesTemp[3][0] = l.x;               mBoxVertexesTemp[3][1] = l.y + s.height;    mBoxVertexesTemp[3][2] = l.z; 
    mBoxVertexesTemp[4][0] = l.x + s.width;     mBoxVertexesTemp[4][1] = l.y;               mBoxVertexesTemp[4][2] = l.z; 
    mBoxVertexesTemp[5][0] = l.x + s.width;     mBoxVertexesTemp[5][1] = l.y + s.height;    mBoxVertexesTemp[5][2] = l.z; 
    
    glColor4f(c->r, c->g, c->b, c->a);
    glVertexPointer(3, GL_FLOAT, 0, mBoxVertexesTemp);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // draw borders if need
    boxColorTemp = ms_palette_get_color(palette, 0);
    
    if(box->border.left)
        ms_gl_draw_left_border(box, boxColorTemp);
    
    if(box->border.top)
        ms_gl_draw_top_border(box, boxColorTemp);
    
    if(box->border.right)
        ms_gl_draw_right_border(box, boxColorTemp);
    
    if(box->border.bottom)
        ms_gl_draw_bottom_border(box, boxColorTemp);
}



void _drawLine(float v[6], color *color)
{
    v[2] += 0.1f;
    v[5] += 0.1f;
    glColor4f(color->r, color->g, color->b, color->a);
    glVertexPointer(3, GL_FLOAT, 0, v);
    glDrawArrays(GL_LINES, 0, 2);
}

void ms_gl_draw_left_border(MsBox *box, color *color)
{
    mBoxBordersTemp[0] = box->location.x;
    mBoxBordersTemp[1] = box->location.y;    
    mBoxBordersTemp[2] = box->location.z; 
    mBoxBordersTemp[3] = box->location.x;
    mBoxBordersTemp[4] = box->location.y + box->size.height;
    mBoxBordersTemp[5] = box->location.z; 
    
    _drawLine(mBoxBordersTemp, color);
}

void ms_gl_draw_top_border(MsBox *box, color *color)
{
    mBoxBordersTemp[0] = box->location.x;
    mBoxBordersTemp[1] = box->location.y;    
    mBoxBordersTemp[2] = box->location.z;
    mBoxBordersTemp[3] = box->location.x + box->size.width;
    mBoxBordersTemp[4] = box->location.y;
    mBoxBordersTemp[5] = box->location.z;
    
    _drawLine(mBoxBordersTemp, color);}

void ms_gl_draw_right_border(MsBox *box, color *color)
{
    mBoxBordersTemp[0] = box->location.x+ box->size.width;;
    mBoxBordersTemp[1] = box->location.y;    
    mBoxBordersTemp[2] = box->location.z;
    mBoxBordersTemp[3] = box->location.x + box->size.width;
    mBoxBordersTemp[4] = box->location.y + box->size.height;
    mBoxBordersTemp[5] = box->location.z;
    
    _drawLine(mBoxBordersTemp, color);  
}

void ms_gl_draw_bottom_border(MsBox *box, color *color)
{
    mBoxBordersTemp[0] = box->location.x + box->size.width;
    mBoxBordersTemp[1] = box->location.y + box->size.height;    
    mBoxBordersTemp[2] = box->location.z;
    mBoxBordersTemp[3] = box->location.x;
    mBoxBordersTemp[4] = box->location.y + box->size.height;
    mBoxBordersTemp[5] = box->location.z;
    
    _drawLine(mBoxBordersTemp, color);
}

void ms_gl_draw_boxgrid(MsBoxGrid *boxGrid)
{
    color  *boxColorTemp;
    
    for(int y = 0; y < boxGrid->grid->rowCount; y ++)
    {
        for(int x = 0; x < boxGrid->grid->columnCount; x ++)
        {
            MsBox* box = (MsBox*)ms_grid_get_item(boxGrid->grid, y, x).p;
            
            
            
            
            if(box->animations->list->count > 0)
            {
                glPushMatrix();
                
                boxColorTemp = ms_palette_get_color(boxGrid->palette, box->boxToAnimate->colorIndex);                
                
                // todo: refactor this
                MsAnimation *animation;
                MsListItem *item = box->animations->list->first;
                while(item != 0)
                {
                    animation = (MsAnimation*)item->value.p;
                    item = item->next;
                    
                    if(animation->rAngle != 0)
                    {
                        glTranslatef(0, +box->boxToAnimate->location.y, 0);
                        glRotatef(animation->rAngle, animation->rVector.x, animation->rVector.y, animation->rVector.z);
                        glTranslatef(0, -box->boxToAnimate->location.y, 0);
                        
                    }
                }
                
                ms_gl_draw_box(boxGrid->palette, box->boxToAnimate, boxColorTemp);

                
                glPopMatrix();
            }
            else if(ms_box_is_visible(box))
            {
                boxColorTemp = ms_palette_get_color(boxGrid->palette, box->colorIndex);
                ms_gl_draw_box(boxGrid->palette, box, boxColorTemp);
            }            
                        
            ms_animationbundle_perform_step(box->animations);
        }
    }
    
}

