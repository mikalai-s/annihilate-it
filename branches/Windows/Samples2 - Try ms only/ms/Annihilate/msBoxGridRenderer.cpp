/*
 *  MsOpenGL.c
 *  AnnihilateIt
 *
 *  Created by user on 11-03-05.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "msBoxGridRenderer.h"

GLfloat mBoxVertexesTemp[4][3]; // 6 vertexes for defining quad by means of two triangles (2 vertex for each)
GLfloat mBoxColorsTemp[4][4];

float mBoxBordersTemp[6]; // array of vertexes for box borders

msBoxGridRenderer::msBoxGridRenderer(msBoxGrid *boxGrid1)
{
	boxGrid = boxGrid1;
}

static const GLuint g_indices[] = { 0, 1, 2, 3 };

void msBoxGridRenderer::drawBox(msPalette *palette, msBox *box, msColor *c)
{
	m_program->use();
    
    msPoint l = box->m_location;
    msSize s = box->m_size;
    mBoxVertexesTemp[0][0] = l.x;               mBoxVertexesTemp[0][1] = l.y;               mBoxVertexesTemp[0][2] = l.z; 
    mBoxVertexesTemp[1][0] = l.x + s.width;     mBoxVertexesTemp[1][1] = l.y;               mBoxVertexesTemp[1][2] = l.z; 
    mBoxVertexesTemp[2][0] = l.x;               mBoxVertexesTemp[2][1] = l.y + s.height;    mBoxVertexesTemp[2][2] = l.z; 
    mBoxVertexesTemp[3][0] = l.x + s.width;     mBoxVertexesTemp[3][1] = l.y + s.height;    mBoxVertexesTemp[3][2] = l.z;    

	for(int i = 0; i < 4; i ++)
	{
		mBoxColorsTemp[i][0] = c->r;
		mBoxColorsTemp[i][1] = c->g;
		mBoxColorsTemp[i][2] = c->b;
		mBoxColorsTemp[i][3] = c->a;
	}
	
	m_program->getAttribute("position")->setPointerAndEnable( 3, GL_FLOAT, 0, 0, mBoxVertexesTemp );
	m_program->getAttribute("color")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, mBoxColorsTemp );

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    
    // draw borders if need   
    if(box->m_border.left)
        drawLeftBorder(box, &box->m_border.color);
    
    if(box->m_border.top)
        drawTopBorder(box, &box->m_border.color);
    
    if(box->m_border.right)
        drawRightBorder(box, &box->m_border.color);
    
    if(box->m_border.bottom)
        drawBottomBorder(box, &box->m_border.color);
}

void msBoxGridRenderer::_drawLine(msPoint &start, msPoint &end, msColor *color)
{
	GLfloat coords[] = {start.x, start.y, start.z /*+ 0.1f*/, end.x, end.y, end.z /*+ 0.1f*/};
	GLfloat colors[] = {color->r, color->g, color->b, color->a, color->r, color->g, color->b, color->a};

	m_program->getAttribute("position")->setPointerAndEnable( 3, GL_FLOAT, 0, 0, coords );
	m_program->getAttribute("color")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, colors );
    
    glDrawArrays(GL_LINES, 0, 2);
}

void msBoxGridRenderer::drawLeftBorder(msBox *box, msColor *color)
{
	msPoint start = box->m_location;
	msPoint end = box->m_location;
	end.y += box->m_size.height;
    
    _drawLine(start, end, color);
}

void msBoxGridRenderer::drawTopBorder(msBox *box, msColor *color)
{
	msPoint start = box->m_location;
	msPoint end = box->m_location;
	end.x += box->m_size.width;

	_drawLine(start, end, color);
}

void msBoxGridRenderer::drawRightBorder(msBox *box, msColor *color)
{
	msPoint start = box->m_location;
	start.x += box->m_size.width;
	msPoint end = box->m_location;
	end.x += box->m_size.width;
	end.y += box->m_size.height;

	_drawLine(start, end, color);
}

void msBoxGridRenderer::drawBottomBorder(msBox *box, msColor *color)
{
	msPoint start = box->m_location;
	start.x += box->m_size.width;
	start.y += box->m_size.height;
	msPoint end = box->m_location;
	end.y += box->m_size.height;

	_drawLine(start, end, color);
}

void msBoxGridRenderer::draw(msShaderProgram *program)
{
	m_program = program;

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
                drawBox(boxGrid->m_palette, box->m_boxToAnimate, boxColorTemp);
            }
            else if(box->isVisible())
            {
                boxColorTemp = boxGrid->m_palette->getColor(box->m_colorIndex);
                drawBox(boxGrid->m_palette, box, boxColorTemp);
            }            
                        
            box->m_animations->performStep();
        }
    }
}

