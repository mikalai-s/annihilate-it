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
#include "../msMatrix.h"
#include "../msMatrixTransform.h"

static const GLfloat g_fbVertexPositions2[] = {
	-1.f, -1.f, 1.f, 1.f,
	 1.f, -1.f, 1.f, 1.f,
	-1.f,  1.f, 1.f, 1.f,
	 1.f,  1.f, 1.f, 1.f,
};

GLfloat textureOrientation[] = {
// left
	0.f, 0.f,
	1.f, 0.f,
	0.f, 1.f,
	1.f, 1.f,
// bottom
	1.f, 0.f,
	1.f, 1.f,
	0.f, 0.f,
	0.f, 1.f,
// right
	1.f, 1.f,
	0.f, 1.f,
	1.f, 0.f,
	0.f, 0.f,
// top
	0.f, 1.f,
	0.f, 0.f,
	1.f, 1.f, 
	1.f, 0.f,
};

static const GLubyte g_fbIndices2[] = {
	0, 1, 2, 3,
};

GLuint borderBuffer;
GLuint vertexBuffer;

msBoxGridRenderer::msBoxGridRenderer(msShaderPrograms *shaders)
{
	m_shaders = shaders;
    
    m_animate = 0;

	_menuBarHeight = 0.1;

    // calculate projection matrix
    float fovy = 45.0f / 180.0f * 3.1415926f;
    float z = -0.5f / tanf(fovy / 2.0f);

    msMatrixTransform transform;
    transform.translate(-0.5f, -0.5f, z)
        ->perspective(fovy, 1.0f, -1.0f, 1.0f)
        ->viewport(2.0f, 2.0f)          
        ->translate(-1.0f, -1.0f, 0.0f);
    m_projectionMatrix = *transform.getMatrix();

    glViewport(0, 0, m_size.width, m_size.height);
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

static const GLubyte g_frontFaceIndices[] = { 0, 1, 2, 3 };
static const GLubyte g_backFaceIndices[] = { 1, 0, 3, 2 };


#define MS_BORDER_LEFT 1
#define MS_BORDER_TOP 2
#define MS_BORDER_RIGTH 4
#define MS_BORDER_BOTTOM 8

void msBoxGridRenderer::drawBox(msShaderProgram *program, msPalette *palette, msBox *box, msBoxFaceData *faceData, bool front)
{
	program->getUniform("borderLineTex")->set1i(program->getTexture("borderLineTex")->getUnit());
	program->getUniform("borderCornerTex")->set1i(program->getTexture("borderCornerTex")->getUnit());

	program->getUniform("lineBorder")->set4iv(1, faceData->getHasBorder());

	msColor faceColor = *palette->getColor(faceData->getColorIndex());
	faceColor.r *= faceData->getColorDisturbance().r;
	faceColor.g *= faceData->getColorDisturbance().g;
	faceColor.b *= faceData->getColorDisturbance().b;
	faceColor.a *= faceData->getColorDisturbance().a;
	program->getUniform("color")->set4fv(1, (GLfloat*)&faceColor);


	msMatrixTransform transform;
    float boxAngle = box->getAngle();
    if(boxAngle != 0.0f)
    {
        msPointf angleVector = box->getAngleVector();
        msPointf center = box->getVerticesData()->getCenter();
	    transform.translate(-center.x, -center.y, -center.z)
		    ->rotate(boxAngle, angleVector)
		    ->translate(center.x, center.y, center.z);
    }

    transform.multiplyMatrix(m_projectionMatrix);
    
	program->getUniform("mvp")->setMatrix4fv(1, false, transform.getMatrix()->getArray());
	
	program->getUniform("cornerBorder")->set4iv(1, faceData->getHasCornerBorder());

    program->getAttribute("position")->setPointerAndEnable( 3, GL_FLOAT, 0, 0, box->getVerticesData()->vertices);

	program->getAttribute("borderTexelLeft")->setPointerAndEnable(2, GL_FLOAT, GL_FALSE, 0, textureOrientation);
	program->getAttribute("borderTexelBottom")->setPointerAndEnable(2, GL_FLOAT, GL_FALSE, 0, &textureOrientation[8]);
	program->getAttribute("borderTexelRight")->setPointerAndEnable(2, GL_FLOAT, GL_FALSE, 0, &textureOrientation[16]);
	program->getAttribute("borderTexelTop")->setPointerAndEnable(2, GL_FLOAT, GL_FALSE, 0, &textureOrientation[24]);

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_frontFaceIndices);

	glDisable(GL_BLEND);
	/*
    // draw borders if need
	msColor innerBorderColor(
		faceColor.r * 0.8f * faceData->getColorDisturbance().r, 
		faceColor.g * 0.8f * faceData->getColorDisturbance().g, 
		faceColor.b * 0.8f * faceData->getColorDisturbance().b, 
		faceColor.a * faceData->getColorDisturbance().a);
	if(faceData->hasLeft())
		drawLeftBorder(program, box, &innerBorderColor);
	if(faceData->hasTop())
		drawTopBorder(program, box, &innerBorderColor);
	if(faceData->hasRight())
		drawRightBorder(program, box, &innerBorderColor);
	if(faceData->hasBottom())
		drawBottomBorder(program, box, &innerBorderColor);
	
    if(!faceData->hasLeft())
        drawLeftBorder(program, box, palette->getColor(0));
    if(!faceData->hasTop())
        drawTopBorder(program, box, palette->getColor(0));
    if(!faceData->hasRight())
        drawRightBorder(program, box, palette->getColor(0));
    if(!faceData->hasBottom())
        drawBottomBorder(program, box, palette->getColor(0));
		*/
	glDisable(GL_CULL_FACE);	
}

void msBoxGridRenderer::_drawLine(msShaderProgram *m_program, msPointf &start, msPointf &end, msColor *color)
{
	GLfloat coords[] = {start.x, start.y, start.z /*+ 0.1f*/, end.x, end.y, end.z /*+ 0.1f*/};

	m_program->getAttribute("position")->setPointerAndEnable( 3, GL_FLOAT, 0, 0, coords );
	m_program->getUniform("color")->set4fv( 1, (GLfloat*)color );
    
    glDrawArrays(GL_LINES, 0, 2);
}

void msBoxGridRenderer::drawLeftBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{    
    _drawLine(m_program, box->getVerticesData()->vertices[0], box->getVerticesData()->vertices[2], color);
}

void msBoxGridRenderer::drawTopBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	_drawLine(m_program, box->getVerticesData()->vertices[0], box->getVerticesData()->vertices[1], color);
}

void msBoxGridRenderer::drawRightBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	_drawLine(m_program, box->getVerticesData()->vertices[1], box->getVerticesData()->vertices[3], color);
}


void msBoxGridRenderer::drawBottomBorder(msShaderProgram *m_program, msBox *box, msColor *color)
{
	_drawLine(m_program, box->getVerticesData()->vertices[2], box->getVerticesData()->vertices[3], color);
}

GLuint gi = 0;

void msBoxGridRenderer::draw( msBoxGrid *boxGrid, msSize size )
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_size = size;

    if(boxGrid == 0)
        return;

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
   glCullFace(GL_FRONT);	

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
				glCullFace(GL_FRONT);
				drawBox(program, boxGrid->m_palette, box, &box->getVerticesData()->frontFace, true);
				
				glCullFace(GL_BACK);
				drawBox(program, boxGrid->m_palette, box, &box->getVerticesData()->backFace, false);
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
	-1.0f, -1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f, 1.0f,
	1.0f,  1.0f,  1.0f, 1.0f,
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
    msTexture *renderTex = m_shaders->getProgramByName("boxgrid")->getFrameBuffer("renderTex")->getTexture();
    renderTex->active();
	renderTex->bind();

    program = m_shaders->getProgramByName("shockwave");
    program->use();

	program->getUniform("tex")->set1i(renderTex->getUnit());
	program->getAttribute("position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, g_vertexPositions );
	program->getAttribute("texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, textureOrientation );

	// wave
	for(msWaveIterator i = m_waves.begin(); i != m_waves.end(); i ++)
	{
		(*i)->render(program);
		(*i)->step();
	}

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_indices );
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
    particleCompleteProgram->getAttribute("a2_texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, textureOrientation );

    // draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
    glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_fbIndices2 );	

    glDisable(GL_BLEND);
}

msWaveEmitter* msBoxGridRenderer::_createWave( msBox* box)
{
	msPointf location = box->getVerticesData()->getCenter();	
	location.x *= m_size.width;
	location.y = 1.0f - location.y;
	location.y *= m_size.height;

	return new msWaveEmitter(location, m_size);
}
