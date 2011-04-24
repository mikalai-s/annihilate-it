/*
 *  MsOpenGL.c
 *  AnnihilateIt
 *
 *  Created by user on 11-03-05.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "msBoxGridRenderer.h"

msBoxGridRenderer::msBoxGridRenderer(msShaderPrograms *shaders)
{
	m_shaders = shaders;

    m_afterShockRadius = -1.0f;
    m_afterShockPower = 100.0f;
    m_afterShockLocation[0] = 0.0f;
    m_afterShockLocation[1] = 0.0f;
    m_animate = 0;

    m_afterShockRadiusMin = 0.0f;
    m_afterShockRadiusMax = 500.0f;
    m_afterShockRadiusStep = 10.0f;
}

msBoxGridRenderer::~msBoxGridRenderer()
{
    for(msExplosionIterator ei = m_explosions.begin(); ei != m_explosions.end(); ei ++)
    {
        delete (*ei);
    }
}

static const GLuint g_indices[] = { 0, 1, 2, 3 };

void msBoxGridRenderer::drawBox(msShaderProgram *m_program, msPalette *palette, msBox *box, msColor *c)
{
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
    //glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_indices);

    
    // draw borders if need   
    if(box->m_border->left)
        drawLeftBorder(m_program, box, &box->m_border->color);
    
    if(box->m_border->top)
        drawTopBorder(m_program, box, &box->m_border->color);
    
    if(box->m_border->right)
        drawRightBorder(m_program, box, &box->m_border->color);
    
    if(box->m_border->bottom)
        drawBottomBorder(m_program, box, &box->m_border->color);
}

void msBoxGridRenderer::_drawLine(msShaderProgram *m_program, msPoint &start, msPoint &end, msColor *color)
{
	GLfloat coords[] = {start.x, start.y, start.z /*+ 0.1f*/, end.x, end.y, end.z /*+ 0.1f*/};
	GLfloat colors[] = {color->r, color->g, color->b, color->a, color->r, color->g, color->b, color->a};

	m_program->getAttribute("position")->setPointerAndEnable( 3, GL_FLOAT, 0, 0, coords );
	m_program->getAttribute("color")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, colors );
    
    glDrawArrays(GL_LINES, 0, 2);
}

void msBoxGridRenderer::drawLeftBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	msPoint start = box->m_location;
	msPoint end = box->m_location;
	end.y += box->m_size.height;
    
    _drawLine(m_program, start, end, color);
}

void msBoxGridRenderer::drawTopBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	msPoint start = box->m_location;
	msPoint end = box->m_location;
	end.x += box->m_size.width;

	_drawLine(m_program, start, end, color);
}

void msBoxGridRenderer::drawRightBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	msPoint start = box->m_location;
	start.x += box->m_size.width;
	msPoint end = box->m_location;
	end.x += box->m_size.width;
	end.y += box->m_size.height;

	_drawLine(m_program, start, end, color);
}


void msBoxGridRenderer::drawBottomBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	msPoint start = box->m_location;
	start.x += box->m_size.width;
	start.y += box->m_size.height;
	msPoint end = box->m_location;
	end.y += box->m_size.height;

	_drawLine(m_program, start, end, color);
}

GLuint gi = 0;

void msBoxGridRenderer::draw(msBoxGrid *boxGrid, msSize size)
{
    m_size = size;

    drawBoxesWithAfterShock(boxGrid, size);
/*
    drawExplosions();

    removeInactiveExplosions();

    // update all animations
    for(int y = 0; y < boxGrid->grid->m_rowCount; y ++)
    {
        for(int x = 0; x < boxGrid->grid->m_columnCount; x ++)
        {
            msBox *box = boxGrid->grid->getItem(y, x);
            box->getAnimated()->getAnimations()->performStep();
        }
    }

    gi ++;*/
}


void msBoxGridRenderer::drawBoxGrid(msShaderProgram *program, msBoxGrid *boxGrid, msSize size)
{
    for(int y = 0; y < boxGrid->grid->m_rowCount; y ++)
    {
        for(int x = 0; x < boxGrid->grid->m_columnCount; x ++)
        {
            msBox *box = boxGrid->grid->getItem(y, x);
            msBoxAnimation *aBox = box->getAnimated();

            // first check for explosion and if box is required one put it into list to be used after grid rendering
            if(aBox->getRequiresExplosion())
            {
                msPoint explosionLocation = box->getAnimated()->m_location;
                explosionLocation.x += box->m_size.width / 2.0f;
                explosionLocation.y += box->m_size.height / 2.0f;
                m_explosions.push_back(_createExplosionPe(explosionLocation, size));
            }
            else
            {
                if(aBox->getAnimations()->getCount() > 0)
                {
                    msColor  boxColorTemp;
                    boxColorTemp = *boxGrid->m_palette->getColor(aBox->m_colorIndex);              
                    boxColorTemp.a *= aBox->m_colorDisturbance.a;
                    boxColorTemp.r *= aBox->m_colorDisturbance.r;
                    boxColorTemp.g *= aBox->m_colorDisturbance.g;
                    boxColorTemp.b *= aBox->m_colorDisturbance.b;
                    
                    drawBox(program, boxGrid->m_palette, aBox, &boxColorTemp);
                }
                else if(box->isVisible())
                {
                    msColor  boxColorTemp;
                    boxColorTemp = *boxGrid->m_palette->getColor(box->m_colorIndex);
                    drawBox(program, boxGrid->m_palette, box, &boxColorTemp);
                }
            }
        }
    }
}

void msBoxGridRenderer::removeInactiveExplosions()
{
    msExplosionList explosionsToDelete;

    for(msExplosionIterator ei = m_explosions.begin(); ei != m_explosions.end(); ei ++)
        if(!(*ei)->isAlive())
            explosionsToDelete.push_back(*ei);

    for(msExplosionIterator ei = explosionsToDelete.begin(); ei != explosionsToDelete.end(); ei ++)
    {
        delete (*ei);
        m_explosions.remove(*ei);
    }
}

static const GLfloat g_fbVertexPositions2[] = {
	-1.0f, -1.0f,  -1.0f,  1.0f,
	1.0f, -1.0f,  -1.0f,  1.0f,
	-1.0f,  1.0f,  -1.0f, 1.0f,
	1.0f,  1.0f,  -1.0f, 1.0f,
};

static const GLfloat g_fbVertexTexcoord2[] = {
	0.0f, 0.f,   
	1.f,  0.f,    
	0.0f, 1.f,    	
	1.f,  1.f,   
};

static const GLubyte g_fbIndices2[] = {
	0, 1, 2, 3,
};

void msBoxGridRenderer::drawExplosions()
{
	// render fire into texture using particle shaders
    msShaderProgram *program = m_shaders->getProgramByName("particle_create");
	program->use();

	// Switch the render target to the current FBO to update the texture map
	program->getFrameBuffer("renderTex")->bind();

	// FBO attachment is complete?
	if (program->getFrameBuffer("renderTex")->isComplete())		
	{
		// Set viewport to size of texture map and erase previous image
		glViewport(0, 0, (GLsizei)m_size.width, (GLsizei)m_size.height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// render particles
        for(msExplosionIterator ei = m_explosions.begin(); ei != m_explosions.end(); ei ++)
        {
            (*ei)->renderParticles(program);
            (*ei)->update(0.015f);
        }
	}

	// Unbind the FBO so rendering will return to the backbuffer.
	m_shaders->getMainFrameBuffer()->bind();

	// usual renderer

	// Set viewport to size of framebuffer and clear color and depth buffers

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind updated texture map
	glActiveTexture(GL_TEXTURE0 + program->getFrameBuffer("renderTex")->getTexture()->getUnit());
	glBindTexture(GL_TEXTURE_2D, program->getFrameBuffer("renderTex")->getTexture()->getId());

	msShaderProgram *particleCompleteProgram = m_shaders->getProgramByName("particle_complete");
	particleCompleteProgram->use();

	particleCompleteProgram->getUniform("u2_texture")->set1i(program->getFrameBuffer("renderTex")->getTexture()->getUnit());
	particleCompleteProgram->getAttribute("a2_position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, g_fbVertexPositions2 );
	particleCompleteProgram->getAttribute("a2_texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, g_fbVertexTexcoord2 );

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_fbIndices2 );	

	glDisable(GL_BLEND);
}

msParticleEmitter* msBoxGridRenderer::_createExplosionPe(msPoint location, msSize screenSize)
{
    float k = (screenSize.width / 320.0f / 2.0f) + (screenSize.height / 480.0f / 2.0f);

    return new msParticleEmitter(
		// explosion
		Vector2fMake(location.x, location.y),//position:
		Vector2fMake(0.031f, 0.031f),//sourcePositionVariance:
		0.0001f,//speed:
		0.007f,//speedVariance:
		0.5f,//particleLifeSpan:
		0.25f,//particleLifespanVariance:
		0.0f,//angle:
		360.0f,//angleVariance:
		Vector2fMake(0.0f, -0.0000025f),//gravity:
		colorMake(1.0f, 0.5f, 0.05f, 1.0f),//startColor:
		colorMake(0.0f, 0.0f, 0.0f, 0.5f),//startColorVariance:
		colorMake(0.2f, 0.0f, 0.0f, 0.0f),//finishColor:
		colorMake(0.2f, 0.0f, 0.0f, 0.0f),//finishColorVariance:
		200,//maxParticles:
		50 * k,//particleSize:
		3 * k,//particleSizeVariance:
		0.125f,//duration:
		GL_TRUE//blendAdditive:
		);
}

static const GLfloat g_vertexPositions[] = {
	-1.0f, -1.0f,  -1.0f,  1.0f,
	1.0f, -1.0f,  -1.0f,  1.0f,
	-1.0f,  1.0f,  -1.0f, 1.0f,
	1.0f,  1.0f,  -1.0f, 1.0f,
};

static const GLfloat g_vertexTexcoord[] = {
	0.0f, 0.f,   
	1.f,  0.f,    
	0.0f, 1.f,    	
	1.f,  1.f,   
};


void msBoxGridRenderer::drawBoxesWithAfterShock(msBoxGrid *boxGrid, msSize size)
{
    // render fire into texture using particle shaders
	msShaderProgram *program = m_shaders->getProgramByName("boxgrid");
	program->use();

	// Switch the render target to the current FBO to update the texture map
	program->getFrameBuffer("renderTex")->bind();

	// FBO attachment is complete?
	if (program->getFrameBuffer("renderTex")->isComplete())
	{
		// Set viewport to size of texture map and erase previous image
		glViewport(0, 0, size.width, size.height);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT );

		// render background
        drawBoxGrid(program, boxGrid, size);
	}

	// Unbind the FBO so rendering will return to the backbuffer.
	m_shaders->getMainFrameBuffer()->bind();

	// usual renderer

	// Set viewport to size of framebuffer and clear color and depth buffers

	// Bind updated texture map
	glActiveTexture(GL_TEXTURE0 + program->getFrameBuffer("renderTex")->getTexture()->getUnit());
	glBindTexture(GL_TEXTURE_2D, program->getFrameBuffer("renderTex")->getTexture()->getId());

    GLuint u = program->getFrameBuffer("renderTex")->getTexture()->getUnit();

    program = m_shaders->getProgramByName("texture_aftershock");
    program->use();

	program->getUniform("tex")->set1i(u);
	program->getAttribute("position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, g_vertexPositions );
	program->getAttribute("texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, g_vertexTexcoord );

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
    glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_indices );	
/*

    // wave

    for(int y = 0; y < boxGrid->grid->m_rowCount; y ++)
    {
        for(int x = 0; x < boxGrid->grid->m_columnCount; x ++)
        {
            msBox *box = boxGrid->grid->getItem(y, x);
            msBoxAnimation *aBox = box->getAnimated();

            if(box->getAnimated()->getRequiresWaveInit())
            {
                msPoint location;
                location.x = box->getAnimated()->m_location.x + box->getAnimated()->m_size.width / 2.0;
                location.x /= 2.0f;
                location.x *= size.width;
                location.y = box->getAnimated()->m_location.y + box->getAnimated()->m_size.height / 2.0;
                location.y /= 2.0f;
                location.y = 1.0f - location.y;
                location.y *= size.height;

                m_afterShockRadius = m_afterShockRadiusMin;
	            m_afterShockPower = 1.0f;
	            m_afterShockLocation[0] = location.x;
	            m_afterShockLocation[1] = location.y;

                break;
            }
            else if(box->getAnimated()->getRequiresWave())
            {
                program->getAttribute("radius")->set1f(m_afterShockRadius);
		        program->getAttribute("power")->set1f(m_afterShockPower);
		        program->getUniform("ep")->set2f(m_afterShockLocation[0], m_afterShockLocation[1]);

                m_afterShockRadius += m_afterShockRadiusStep;
			    m_afterShockPower -= m_afterShockRadiusStep / (m_afterShockRadiusMax - m_afterShockRadiusMin);

                break;
            }
        }
    }


/*	if(m_animate == 1)
	{
		

		c--;
		if(c < 40)
		{
            

			
		}		
	}*/
}



void msBoxGridRenderer::startWave(msPoint location)
{/*
    //c = 50;
    c = 50;
	m_animate = 1;
	m_afterShockRadius = m_afterShockRadiusMin;
	m_afterShockPower = 1.0f;
	m_afterShockLocation[0] = location.x;
	m_afterShockLocation[1] = location.y;
    */
}

