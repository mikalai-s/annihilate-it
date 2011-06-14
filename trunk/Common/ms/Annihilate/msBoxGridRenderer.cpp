/*
 *  MsOpenGL.c
 *  AnnihilateIt
 *
 *  Created by user on 11-03-05.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "msBoxGridRenderer.h"
#include "msSpline.h"

static const GLfloat g_fbVertexPositions2[] = {
	-1.f, -1.f, -1.f, 1.f,
	 1.f, -1.f, -1.f, 1.f,
	-1.f,  1.f, -1.f, 1.f,
	 1.f,  1.f, -1.f, 1.f,
};

static const GLfloat g_fbVertexTexcoord2[] = {
	0.f, 0.f,
	1.f, 0.f, 
	0.f, 1.f,
	1.f, 1.f,
};

GLfloat borderOrientation_left[] = {
	0.f, 0.f, -1.0f, -1.0f,
	1.f, 0.f, -1.0f, -1.0f,
	0.f, 1.f, -1.0f, -1.0f,
	1.f, 1.f, -1.0f, -1.0f,
};


GLfloat borderOrientation_bottom[] = {
	1.f, 0.f, -1.0f, -1.0f,
	1.f, 1.f, -1.0f, -1.0f,
	0.f, 0.f, -1.0f, -1.0f,
	0.f, 1.f, -1.0f, -1.0f,
};

GLfloat borderOrientation_right[] = {
	1.f, 1.f, -1.0f, -1.0f,
	0.f, 1.f, -1.0f, -1.0f,
	1.f, 0.f, -1.0f, -1.0f,
	0.f, 0.f, -1.0f, -1.0f,
};

GLfloat borderOrientation_top[] = {
	0.f, 1.f, -1.0f, -1.0f,
	0.f, 0.f, -1.0f, -1.0f,
	1.f, 1.f, -1.0f, -1.0f,
	1.f, 0.f, -1.0f, -1.0f,
};

static const GLubyte g_fbIndices2[] = {
	0, 1, 2, 3,
};

msBoxGridRenderer::msBoxGridRenderer(msShaderPrograms *shaders)
{
	m_shaders = shaders;
    
    m_animate = 0;

	_menuBarHeight = 0.1;
}

msBoxGridRenderer::~msBoxGridRenderer()
{
    for(msExplosionIterator ei = m_explosions.begin(); ei != m_explosions.end(); ei ++)
    {
        delete (*ei);
    }

	for(msWaveIterator wi = m_waves.begin(); wi != m_waves.end(); wi ++)
	{
		delete (*wi);
	}
}

static const GLubyte g_indices[] = { 0, 1, 2, 3 };

#define MS_BORDER_LEFT 1
#define MS_BORDER_TOP 2
#define MS_BORDER_RIGTH 4
#define MS_BORDER_BOTTOM 8

void msBoxGridRenderer::drawBox(msShaderProgram *m_program, msPalette *palette, msBox *box, msColor *c)
{
    msPointf l = box->getLocation();
    msSize s = box->getSize();

	msPointf points[4];
	points[0] = msPointf(l.x, l.y);
	points[1] = msPointf(l.x + s.width, l.y);
	points[2] = msPointf(l.x, l.y + s.height);
	points[3] = msPointf(l.x + s.width, l.y + s.height);

	msColor colors[4];
	colors[0] = colors[1] = colors[2] = colors[3] = *c;

	glEnable(GL_BLEND);

	m_program->getAttribute("position")->setPointerAndEnable( 3, GL_FLOAT, 0, 0, points );
	m_program->getAttribute("color")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, colors );
	m_program->getUniform("border_line_tex")->set1i(m_program->getTexture("border_line_tex")->getUnit());
	m_program->getUniform("border_corner_tex")->set1i(m_program->getTexture("border_corner_tex")->getUnit());

	borderOrientation_left[3] = borderOrientation_left[7] = borderOrientation_left[11] = borderOrientation_left[15] = box->getLeft() ? -1.0f : 1.0f;
	m_program->getAttribute("borderLineTexelLeft")->setPointerAndEnable(4, GL_FLOAT, 0, 0, borderOrientation_left );

	borderOrientation_bottom[3] = borderOrientation_bottom[7] = borderOrientation_bottom[11] = borderOrientation_bottom[15] = box->getBottom() ? -1.0f : 1.0f;
	m_program->getAttribute("borderLineTexelBottom")->setPointerAndEnable(4, GL_FLOAT, 0, 0, borderOrientation_bottom );

	borderOrientation_right[3] = borderOrientation_right[7] = borderOrientation_right[11] = borderOrientation_right[15] = box->getRight() ? -1.0f : 1.0f;
	m_program->getAttribute("borderLineTexelRight")->setPointerAndEnable(4, GL_FLOAT, 0, 0, borderOrientation_right );

	borderOrientation_top[3] = borderOrientation_top[7] = borderOrientation_top[11] = borderOrientation_top[15] = box->getTop() ? -1.0f : 1.0f;
	m_program->getAttribute("borderLineTexelTop")->setPointerAndEnable(4, GL_FLOAT, 0, 0, borderOrientation_top );


	bool isleft = box->getLeft() && !box->getLeft()->getTop() && box->getTop() && !box->getTop()->getLeft();
	borderOrientation_left[2] = borderOrientation_left[6] = borderOrientation_left[10] = borderOrientation_left[14] = isleft ? 1.0f : -1.0f;
	m_program->getAttribute("borderCornerTexelLeft")->setPointerAndEnable(4, GL_FLOAT, 0, 0, borderOrientation_left );

	bool isBottom = box->getLeft() && !box->getLeft()->getBottom() && box->getBottom() && !box->getBottom()->getLeft();
	borderOrientation_bottom[2] = borderOrientation_bottom[6] = borderOrientation_bottom[10] = borderOrientation_bottom[14] = isBottom ? 1.0f : -1.0f;
	m_program->getAttribute("borderCornerTexelBottom")->setPointerAndEnable(4, GL_FLOAT, 0, 0, borderOrientation_bottom );

	bool isRight = box->getRight() && !box->getRight()->getBottom() && box->getBottom() && !box->getBottom()->getRight();
	borderOrientation_right[2] = borderOrientation_right[6] = borderOrientation_right[10] = borderOrientation_right[14] = isRight ? 1.0f : -1.0f;
	m_program->getAttribute("borderCornerTexelRight")->setPointerAndEnable(4, GL_FLOAT, 0, 0, borderOrientation_right );

	bool isTop = box->getTop() && !box->getTop()->getRight() && box->getRight() && !box->getRight()->getTop();
	borderOrientation_top[2] = borderOrientation_top[6] = borderOrientation_top[10] = borderOrientation_top[14] = isTop ? 1.0f : -1.0f;
	m_program->getAttribute("borderCornerTexelTop")->setPointerAndEnable(4, GL_FLOAT, 0, 0, borderOrientation_top );

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_BLEND);

    // draw borders if need
	msColor innerBorderColor(c->r * 0.8f, c->g * 0.8f, c->b * 0.8f, c->a);
	if(box->getLeft())
		drawLeftBorder(m_program, box, &innerBorderColor);
	if(box->getTop())
		drawTopBorder(m_program, box, &innerBorderColor);
	if(box->getRight())
		drawRightBorder(m_program, box, &innerBorderColor);
	if(box->getBottom())
		drawBottomBorder(m_program, box, &innerBorderColor);
	
    if(!box->getLeft())
        drawLeftBorder(m_program, box, palette->getColor(0));
    if(!box->getTop())
        drawTopBorder(m_program, box, palette->getColor(0));
    if(!box->getRight())
        drawRightBorder(m_program, box, palette->getColor(0));
    if(!box->getBottom())
        drawBottomBorder(m_program, box, palette->getColor(0));
}

void msBoxGridRenderer::_drawLine(msShaderProgram *m_program, msPointf &start, msPointf &end, msColor *color)
{
	GLfloat coords[] = {start.x, start.y, start.z /*+ 0.1f*/, end.x, end.y, end.z /*+ 0.1f*/};
	GLfloat colors[] = {color->r, color->g, color->b, color->a, color->r, color->g, color->b, color->a};

	m_program->getAttribute("position")->setPointerAndEnable( 3, GL_FLOAT, 0, 0, coords );
	m_program->getAttribute("color")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, colors );
    
    glDrawArrays(GL_LINES, 0, 2);
}

void msBoxGridRenderer::drawLeftBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	msPointf start = box->getLocation();
	msPointf end = box->getLocation();
	end.y += box->getSize().height;
    
    _drawLine(m_program, start, end, color);
}

void msBoxGridRenderer::drawTopBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	msPointf start = box->getLocation();
	msPointf end = box->getLocation();
	end.x += box->getSize().width;

	_drawLine(m_program, start, end, color);
}

void msBoxGridRenderer::drawRightBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	msPointf start = box->getLocation();
	start.x += box->getSize().width;
	msPointf end = box->getLocation();
	end.x += box->getSize().width;
	end.y += box->getSize().height;

	_drawLine(m_program, start, end, color);
}


void msBoxGridRenderer::drawBottomBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	msPointf start = box->getLocation();
	start.x += box->getSize().width;
	start.y += box->getSize().height;
	msPointf end = box->getLocation();
	end.y += box->getSize().height;

	_drawLine(m_program, start, end, color);
}

GLuint gi = 0;

void msBoxGridRenderer::draw(msBoxGrid *boxGrid, msSize size)
{
    m_size = size;

    drawBoxesWithShockWave(boxGrid);

    drawExplosions();

    removeInactiveEmitters();

    // update all animations
    for(int y = 0; y < boxGrid->m_rowCount; y ++)
    {
        for(int x = 0; x < boxGrid->m_columnCount; x ++)
        {
            msBox *box = boxGrid->getItem(y, x);
            box->getAnimations()->performStep();
        }
    }

	boxGrid->getAnimations()->performStep();

    gi ++;
}

size_t m_lastExplosions = 0;

void msBoxGridRenderer::showExplosions()
{
    for(msExplosionIterator ei = m_explosions.begin(); ei != m_explosions.end(); ei ++)
    {
        printf("explosions:\t%d\r\n", (*ei)->particleCount);
    }
}

void msBoxGridRenderer::drawBoxGrid(msShaderProgram *program, msBoxGrid *boxGrid, msSize size)
{
    for(int y = 0; y < boxGrid->m_rowCount; y ++)
    {
        for(int x = 0; x < boxGrid->m_columnCount; x ++)
        {
            msBox *box = boxGrid->getItem(y, x);

            // first check for explosion and if box is required one put it into list to be used after grid rendering
            if(box->getRequiresExplosion())
            {
                m_explosions.push_back(_createExplosionPe(box->getExplosionPoint(), size));
            }

			if(box->getRequiresWave())
			{
				m_waves.push_back(_createWave(box));
			}

			if(box->isVisible())
            {
                msColor  boxColorTemp;
                boxColorTemp = *boxGrid->m_palette->getColor(box->getColorIndex());
                drawBox(program, boxGrid->m_palette, box, &boxColorTemp);
            }
        }
    }
}

void msBoxGridRenderer::removeInactiveEmitters()
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

	msWaveList wavesToDelete;

	for(msWaveIterator wi = m_waves.begin(); wi != m_waves.end(); wi ++)
		if(!(*wi)->isAlive())
			wavesToDelete.push_back(*wi);

	for(msWaveIterator wi = wavesToDelete.begin(); wi != wavesToDelete.end(); wi ++)
	{
		delete (*wi);
		m_waves.remove(*wi);
	}
}



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
	program->getFrameBuffer("renderTex")->getTexture()->active();
	program->getFrameBuffer("renderTex")->getTexture()->bind();

	msShaderProgram *particleCompleteProgram = m_shaders->getProgramByName("particle_complete");
	particleCompleteProgram->use();

	particleCompleteProgram->getUniform("u2_texture")->set1i(program->getFrameBuffer("renderTex")->getTexture()->getUnit());
	particleCompleteProgram->getAttribute("a2_position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, g_fbVertexPositions2 );
	particleCompleteProgram->getAttribute("a2_texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, g_fbVertexTexcoord2 );

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_fbIndices2 );	

	glDisable(GL_BLEND);
}

msParticleEmitter* msBoxGridRenderer::_createExplosionPe(msPointf location, msSize screenSize)
{
    float k = (screenSize.width / 320.0f) + (screenSize.height / 480.0f);

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
	0.f, 0.f,
	1.f, 0.f,
	0.f, 1.f,
	1.f, 1.f,
};


void msBoxGridRenderer::drawBoxesWithShockWave(msBoxGrid *boxGrid)
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
		glViewport(0, 0, m_size.width, m_size.height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT );

		// render background
        drawBoxGrid(program, boxGrid, m_size);
	}

	// Unbind the FBO so rendering will return to the backbuffer.
	m_shaders->getMainFrameBuffer()->bind();

	// usual renderer

	// Bind updated texture map
	program->getFrameBuffer("renderTex")->getTexture()->active();
	program->getFrameBuffer("renderTex")->getTexture()->bind();

    GLuint u = program->getFrameBuffer("renderTex")->getTexture()->getUnit();

    program = m_shaders->getProgramByName("shockwave");
    program->use();

	program->getUniform("tex")->set1i(u);
	program->getAttribute("position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, g_vertexPositions );
	program->getAttribute("texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, g_vertexTexcoord );

	// wave
	for(msWaveIterator i = m_waves.begin(); i != m_waves.end(); i ++)
	{
		(*i)->render(program);
		(*i)->step();
	}

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_indices );
}

msWaveEmitter* msBoxGridRenderer::_createWave( msBox* box)
{
	msPointf location;
	location.x = box->getLocation().x + box->getSize().width / 2.0;
	location.x *= m_size.width;
	location.y = box->getLocation().y + box->getSize().height / 2.0;
	location.y = 1.0f - location.y;
	location.y *= m_size.height;

	return new msWaveEmitter(location, m_size);
}
