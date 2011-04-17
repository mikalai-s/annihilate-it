/*
 *  MsOpenGL.c
 *  AnnihilateIt
 *
 *  Created by user on 11-03-05.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "msBoxGridRenderer.h"

GLfloat mBoxVertexesTemp[6][3]; // 6 vertexes for defining quad by means of two triangles (2 vertex for each)
GLfloat mBoxColorsTemp[6][4];

float mBoxBordersTemp[6]; // array of vertexes for box borders

msBoxGridRenderer::msBoxGridRenderer(msBoxGrid *boxGrid1)
{
	boxGrid = boxGrid1;
}

static const GLuint g_indices[] = { 0, 1, 2, 3 };

void msBoxGridRenderer::drawBox(msShaderProgram *program, msPalette *palette, msBox *box, msColor *c)
{
	program->use();
    
    msPoint l = box->m_location;
    msSize s = box->m_size;
    mBoxVertexesTemp[0][0] = l.x;               mBoxVertexesTemp[0][1] = l.y;               mBoxVertexesTemp[0][2] = l.z; 
    mBoxVertexesTemp[1][0] = l.x + s.width;     mBoxVertexesTemp[1][1] = l.y;               mBoxVertexesTemp[1][2] = l.z; 
    mBoxVertexesTemp[2][0] = l.x;               mBoxVertexesTemp[2][1] = l.y + s.height;    mBoxVertexesTemp[2][2] = l.z; 
    mBoxVertexesTemp[3][0] = l.x;               mBoxVertexesTemp[3][1] = l.y + s.height;    mBoxVertexesTemp[3][2] = l.z; 
    mBoxVertexesTemp[4][0] = l.x + s.width;     mBoxVertexesTemp[4][1] = l.y;               mBoxVertexesTemp[4][2] = l.z; 
    mBoxVertexesTemp[5][0] = l.x + s.width;     mBoxVertexesTemp[5][1] = l.y + s.height;    mBoxVertexesTemp[5][2] = l.z; 

	for(int i = 0 ;i < 6; i ++)
	{
		mBoxColorsTemp[i][0] = c->r;
		mBoxColorsTemp[i][1] = c->g;
		mBoxColorsTemp[i][2] = c->b;
		mBoxColorsTemp[i][3] = c->a;
	}
    
	
	program->getAttribute("position")->setPointerAndEnable( 3, GL_FLOAT, 0, 0, mBoxVertexesTemp );
	program->getAttribute("color")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, mBoxColorsTemp );

	glDrawArrays(GL_TRIANGLES, 0, 6);
/*

    
    // draw borders if need
    boxColorTemp = palette->getColor(0);
    
    if(box->border.left)
        ms_gl_draw_left_border(box, boxColorTemp);
    
    if(box->border.top)
        ms_gl_draw_top_border(box, boxColorTemp);
    
    if(box->border.right)
        ms_gl_draw_right_border(box, boxColorTemp);
    
    if(box->border.bottom)
        ms_gl_draw_bottom_border(box, boxColorTemp);*/
}



void msBoxGridRenderer::_drawLine(float v[6], msColor *color)
{/*
    v[2] += 0.1f;
    v[5] += 0.1f;
    glColor4f(color->r, color->g, color->b, color->a);
    glVertexPointer(3, GL_FLOAT, 0, v);
    glDrawArrays(GL_LINES, 0, 2);
	*/
}

void msBoxGridRenderer::drawLeftBorder(msBox *box, msColor *color)
{
    mBoxBordersTemp[0] = box->m_location.x;
    mBoxBordersTemp[1] = box->m_location.y;    
    mBoxBordersTemp[2] = box->m_location.z; 
    mBoxBordersTemp[3] = box->m_location.x;
    mBoxBordersTemp[4] = box->m_location.y + box->m_size.height;
    mBoxBordersTemp[5] = box->m_location.z; 
    
    _drawLine(mBoxBordersTemp, color);
}

void msBoxGridRenderer::drawTopBorder(msBox *box, msColor *color)
{
    mBoxBordersTemp[0] = box->m_location.x;
    mBoxBordersTemp[1] = box->m_location.y;    
    mBoxBordersTemp[2] = box->m_location.z;
    mBoxBordersTemp[3] = box->m_location.x + box->m_size.width;
    mBoxBordersTemp[4] = box->m_location.y;
    mBoxBordersTemp[5] = box->m_location.z;
    
    _drawLine(mBoxBordersTemp, color);}

void msBoxGridRenderer::drawRightBorder(msBox *box, msColor *color)
{
    mBoxBordersTemp[0] = box->m_location.x+ box->m_size.width;;
    mBoxBordersTemp[1] = box->m_location.y;    
    mBoxBordersTemp[2] = box->m_location.z;
    mBoxBordersTemp[3] = box->m_location.x + box->m_size.width;
    mBoxBordersTemp[4] = box->m_location.y + box->m_size.height;
    mBoxBordersTemp[5] = box->m_location.z;
    
    _drawLine(mBoxBordersTemp, color);  
}

void msBoxGridRenderer::drawBottomBorder(msBox *box, msColor *color)
{
    mBoxBordersTemp[0] = box->m_location.x + box->m_size.width;
    mBoxBordersTemp[1] = box->m_location.y + box->m_size.height;    
    mBoxBordersTemp[2] = box->m_location.z;
    mBoxBordersTemp[3] = box->m_location.x;
    mBoxBordersTemp[4] = box->m_location.y + box->m_size.height;
    mBoxBordersTemp[5] = box->m_location.z;
    
    _drawLine(mBoxBordersTemp, color);
}

void msBoxGridRenderer::draw(msShaderProgram *program)
{
    msColor  *boxColorTemp;
    
    for(int y = 0; y < boxGrid->grid->m_rowCount; y ++)
    {
        for(int x = 0; x < boxGrid->grid->m_columnCount; x ++)
        {
            msBox* box = boxGrid->grid->getItem(y, x);
            
            if(box->m_animations->m_list.size() > 0)
            {
                boxColorTemp = boxGrid->m_palette->getColor(box->m_boxToAnimate->m_colorIndex);                
                
                // todo: refactor this
				/*
                msListItem *item = box->animations->list->first;
                while(item != 0)
                {
                    msAnimation *animation = (msAnimation*)item->value.p;
                    item = item->next;
                    
                    if(animation->rAngle != 0)
                    {
                        glTranslatef(0, +box->boxToAnimate->location.y, 0);
                        glRotatef(animation->rAngle, animation->rVector.x, animation->rVector.y, animation->rVector.z);
                        glTranslatef(0, -box->boxToAnimate->location.y, 0);
                        
                    }
                }
                */
                drawBox(program, boxGrid->m_palette, box->m_boxToAnimate, boxColorTemp);
            }
            else if(box->isVisible())
            {
                boxColorTemp = boxGrid->m_palette->getColor(box->m_colorIndex);
                drawBox(program, boxGrid->m_palette, box, boxColorTemp);
            }            
                        
            box->m_animations->performStep();
        }
    }
    
}

