//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation for the Scene class.  This class wraps in drawing in GL.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Scene.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "msScene.h"
#include "ShaderProgram/msShaderPrograms.h"
#include "msParticleEmitter.h"

using namespace std;

//=================================================================================================================================
///
/// Constructor
///
/// \param none
///
/// \return null
//=================================================================================================================================
msScene::msScene()
{
   GLfloat color[] = { 0.3f, 0.3f, 0.7f, 1.0f };
   m_clearColor[0] = color[0];
   m_clearColor[1] = color[1];
   m_clearColor[2] = color[2];
   m_clearColor[3] = color[3];

   m_afterShockRadius = -1.0f;
   m_afterShockPower = 1.0f;
   m_afterShockLocation[0] = 0.0f;
   m_afterShockLocation[1] = 0.0f;
   m_animate = 0;

   m_afterShockRadiusMin = 0.0f;
   m_afterShockRadiusMax = 1500.0f;
   m_afterShockRadiusStep = 37.0f;

   m_explosionParticles = 0;
   pe2 = 0;
   pe3 = 0;

   m_palette = 0;
   m_boxGrid = 0;
   m_renderer = 0;
}

void msScene::newSize(GLint width, GLint height)
{
	_width = width;
	_height = height;
	
	m_shaders.notifySizeChanged(width, height);	

    delete m_explosionParticles;
    m_explosionParticles = _createExplosionPe(width, height);
}

//=================================================================================================================================
///
/// Destructor
///
/// \param void
///
/// \return void
//=================================================================================================================================
msScene::~msScene()
{
	if(m_explosionParticles != 0)
		delete m_explosionParticles;
	if(pe2 != 0)
		delete pe2;
	if(pe3 != 0)
		delete pe3;

    if(m_palette != 0)
	    delete m_palette;
    if(m_boxGrid != 0)
	    delete m_boxGrid;
    if(m_renderer != 0)
	    delete m_renderer;
}

//=================================================================================================================================
///
/// Loads all the data from the file (textures, attributes, uniforms, shaders...)
///
/// \param fileName is the name for the file where we get the data
///
/// \return bool saying whether we passed or failed
//=================================================================================================================================
bool msScene::loadData(string filename)
{
   m_shaders.loadFile(filename);
   return true;
}






//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// More definitions for the Scene class.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/esTriangle/src/Draw.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================





//=================================================================================================================================
///
/// Draws the current frame.
///
/// \param none
///
/// \return null
//=================================================================================================================================

msParticleEmitter* msScene::_createExplosionPe(GLint _width, GLint _height)
{
    float k = (_width / 320.0f / 2.0f) + (_height / 480.0f / 2.0f);

    return new msParticleEmitter(
		// explosion
		Vector2fMake(0.0f, 0.0f),//position:
		Vector2fMake(0.031f, 0.031f),//sourcePositionVariance:
		0.001f,//speed:
		0.007f,//speedVariance:
		0.5f,//particleLifeSpan:
		0.25f,//particleLifespanVariance:
		0.0f,//angle:
		360.0f,//angleVariance:
		Vector2fMake(0.0f, -0.000025f),//gravity:
		colorMake(1.0f, 0.5f, 0.05f, 1.0f),//startColor:
		colorMake(0.0f, 0.0f, 0.0f, 0.5f),//startColorVariance:
		colorMake(0.2f, 0.0f, 0.0f, 0.0f),//finishColor:
		colorMake(0.2f, 0.0f, 0.0f, 0.0f),//finishColorVariance:
		200,//maxParticles:
		50 * k,//particleSize:
		3 * k,//particleSizeVariance:
		-1,//0.125f,//duration:
		GL_TRUE//blendAdditive:
		);
}

GLfloat colorMap[][4] = 
{
	//{1.0f, 1.0f, 1.0f, 1.0f},//temp white
	{0.000f, 0.000f, 0.000f, 1.0f}, // black
	{0.502f, 0.824f, 0.776f, 1.0f}, // 7
	{1.000f, 0.412f, 0.337f, 1.0f}, // 1    
	{0.467f, 0.357f, 1.000f, 1.0f}, // 3
	{1.000f, 0.757f, 0.369f, 1.0f}, // 2
	{0.843f, 0.420f, 1.000f, 1.0f}, // 6
	{0.478f, 0.824f, 1.000f, 1.0f}, // 5
	{0.714f, 1.000f, 0.608f, 1.0f}, // 4    
};

void msScene::init()
{
	m_shaders.notifySizeChanged(_width, _height);

	pe2 = new msParticleEmitter(
		// dust
		Vector2fMake(0.0f, 0.0f),//position:
		Vector2fMake(0.1f, 0.0f),//sourcePositionVariance:
		0.001f,//speed:
		0.00005f,//speedVariance:
		0.5f,//particleLifeSpan:
		0.5f,//particleLifespanVariance:
		90.0f,//angle:
		90.0f,//angleVariance:
		Vector2fMake(0.0f, -0.0001f),//gravity:
		colorMake(0.5f, 0.5f, 0.5f, 1.0f),//startColor:
		colorMake(0.0f, 0.0f, 0.0f, 0.5f),//startColorVariance:
		colorMake(0.1f, 0.1f, 0.1f, 0.0f),//finishColor:
		colorMake(0.0f, 0.0f, 0.0f, 0.0f),//finishColorVariance:
		1000,//maxParticles:
		20,//particleSize:
		5,//particleSizeVariance:
		-1,//0.125f,//duration:
		GL_FALSE//blendAdditive:
		);

	pe3 = 0;


	// init palette
	m_palette = new msPalette(colorMap, 8);

	m_boxGrid = new msBoxGrid(m_palette, 4, NUM_ROWS, NUM_COLS, 2.0, 2.0);

	m_renderer = new msBoxGridRenderer(&m_shaders);
}

int c = 0;

static const GLfloat g_fbVertexPositions[] = {
	0.0f, 0.0f,  
	1.0f, 0.0f,  
	0.0f,  1.0f,  
	1.0f,  1.0f,  
};

static const GLfloat g_fbVertexTexcoord[] = {
	0.0f, 0.0f,   
	1.0f,  0.0f,    
	0.0f, 1.0f,    	
	1.0f,  1.0f,   
};

static const GLubyte g_fbIndices[] = {
	0, 1, 2, 3,
};


static const GLfloat g_vertexPositions[] = {
	-1.0f, -0.7f,
	0.7f, -0.7f,
	-0.7f,  0.7f,
	0.5f,  0.5f, 
};

static const GLfloat g_vertexColors[] = {
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
};

static const GLfloat g_vertexTexcoord[] = {
	0.0f, 0.f,   
	1.f,  0.f,    
	0.0f, 1.f,    	
	1.f,  1.f,   
};

static const GLubyte g_indices[] = {
	0, 1, 2, 3,
};

static const GLfloat prim[] =
{
	0.f, 0.f, -1.f, 1.0f,
	0.f, 1.f, -1.f, 1.0f,
	1.f, 0.f, -1.f, 1.0f,
	1.f, 1.f, -1.f, 1.0f,	
};



void msScene::drawBackground()
{
	// render fire into texture using particle shaders
	msShaderProgram *program = m_shaders.getProgramByName("boxgrid");
	program->use();

	// Switch the render target to the current FBO to update the texture map
	program->getFrameBuffer("renderTex")->bind();

	// FBO attachment is complete?
	if (program->getFrameBuffer("renderTex")->isComplete())
	{
		// Set viewport to size of texture map and erase previous image
		glViewport(0, 0, _width, _height);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT );

		// render background
		//program->getUniform("tex")->set1i(program->getTexture("tex0")->getUnit());
		program->getAttribute("position")->setPointerAndEnable( 2, GL_FLOAT, 0, 0, g_vertexPositions );
		program->getAttribute("color")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, g_vertexColors );
		//program->getAttribute("texcoord")->setPointerAndEnable( 2, GL_FLOAT, 0, 0, g_vertexTexcoord );

		// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
		glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_indices );

		//program->getUniform("tex")->set1i(program->getTexture("ms0")->getUnit());
		//program->getAttribute("position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, prim);

		//glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_indices);
	}

	// Unbind the FBO so rendering will return to the backbuffer.
	m_shaders.getMainFrameBuffer()->bind();
    
    msTexture *renderTex = program->getFrameBuffer("renderTex")->getTexture();
    
    program = m_shaders.getProgramByName("texture_aftershock");
	program->use();

	// usual renderer

	// Set viewport to size of framebuffer and clear color and depth buffers

	// Bind updated texture map
	renderTex->active();
	renderTex->bind();

	program->getUniform("tex")->set1i(renderTex->getUnit());
	program->getAttribute("position")->setPointerAndEnable(2, GL_FLOAT, 0, 0, g_fbVertexPositions );
	program->getAttribute("texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, g_fbVertexTexcoord );

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_fbIndices );	

/*

	if(m_animate)
	{
		program->getAttribute("radius")->set1f(m_afterShockRadius);
		program->getAttribute("power")->set1f(m_afterShockPower);
		program->getUniform("ep")->set2f(m_afterShockLocation[0], m_afterShockLocation[1]);

		c--;
		if(c < 40)
		{
			m_afterShockRadius += m_afterShockRadiusStep;
			m_afterShockPower -= m_afterShockRadiusStep / (m_afterShockRadiusMax - m_afterShockRadiusMin);
			if(m_afterShockRadius > m_afterShockRadiusMax)
			{
				m_animate = 0;
				m_afterShockRadius = -1.0f;
			}
		}		
	}*/
}

void msScene::drawExplosion()
{
	// render fire into texture using particle shaders
	msShaderProgram *program = m_shaders.getProgramByName("particle_create");
	program->use();

	// Switch the render target to the current FBO to update the texture map
	program->getFrameBuffer("renderTex")->bind();

	// FBO attachment is complete?
	if (program->getFrameBuffer("renderTex")->isComplete())		
	{
		// Set viewport to size of texture map and erase previous image
		glViewport(0, 0, _width, _height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// render particles
		m_explosionParticles->renderParticles(program);
		m_explosionParticles->update(0.015f);

		//pe2->renderParticles(program);
		pe2->update(0.015f);
	}

	// Unbind the FBO so rendering will return to the backbuffer.
	m_shaders.getMainFrameBuffer()->bind();

	// usual renderer

	// Set viewport to size of framebuffer and clear color and depth buffers

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind updated texture map
	program->getFrameBuffer("renderTex")->getTexture()->active();
	program->getFrameBuffer("renderTex")->getTexture()->bind();

	msShaderProgram *particleCompleteProgram = m_shaders.getProgramByName("particle_complete");
	particleCompleteProgram->use();

	particleCompleteProgram->getUniform("u2_texture")->set1i(program->getFrameBuffer("renderTex")->getTexture()->getUnit());
	particleCompleteProgram->getAttribute("a2_position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, g_fbVertexPositions );
	particleCompleteProgram->getAttribute("a2_texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, g_fbVertexTexcoord );

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_fbIndices );	

	glDisable(GL_BLEND);
}


void msScene::drawFrame()
{
    m_shaders.getMainFrameBuffer()->bind();
    
	glViewport(0, 0, _width, _height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    msSize size;
    size.width = (GLfloat)_width;
    size.height = (GLfloat)_height;
	m_renderer->draw(m_boxGrid, size);
     

  // drawBackground();

	//drawExplosion();	
/*

	msShaderProgram *program = m_shaders.getProgramByName("texture_aftershock");
	program->use();
	
    // Set viewport to size of texture map and erase previous image
	glViewport(0, 0, _width, _height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT );

	// render background
	program->getUniform("tex")->set1i(program->getTexture("tex0")->getUnit());
	program->getAttribute("position")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, g_vertexPositions );
	program->getAttribute("color")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, g_vertexColors );
	program->getAttribute("texcoord")->setPointerAndEnable( 2, GL_FLOAT, 0, 0, g_vertexTexcoord );

    // draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, g_indices );
    
	program->getUniform("tex")->set1i(program->getTexture("ms0")->getUnit());
	program->getAttribute("position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, prim);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 */
 
}

int getShiftDirection()
{
	return MS_BOX_SHIFT_DOWN;
}

void msScene::mouseClick(int x, int y)
{/*
	c = 50;
	m_animate = 1;
	m_afterShockRadius = m_afterShockRadiusMin;
	m_afterShockPower = 1.0f;
	m_afterShockLocation[0] = (float)x;
	m_afterShockLocation[1] = (float)this->_height - (float)y;
*/
	m_explosionParticles->active = true;
	m_explosionParticles->duration = 0.125f;
	m_explosionParticles->sourcePosition.x = (m_afterShockLocation[0] / (float)this->_width * 2.0f) - 1.0f;
	m_explosionParticles->sourcePosition.y = (m_afterShockLocation[1] / (float)this->_height * 2.0f) - 1.0f;

	pe2->active = true;
	pe2->duration = 0.125f;
	pe2->sourcePosition.x = (m_afterShockLocation[0] / (float)this->_width * 2.0f) - 1.0f;
	pe2->sourcePosition.y = (m_afterShockLocation[1] / (float)this->_height * 2.0f) - 1.0f;

	msPoint touchPoint;
	touchPoint.x = ((GLfloat)x / (GLfloat)_width);
	touchPoint.y = ((GLfloat)y / (GLfloat)_height);
	
	m_boxGrid->removeSimilarItemsAtPoint(touchPoint);

	m_boxGrid->shiftPendentBoxes(getShiftDirection());

    msPoint p;
    p.x = (float)x;
    p.y = (float)this->_height - (float)y;
    m_renderer->startWave(p);
}



void msScene::setMainFrameBuffer(GLint id)
{
	m_shaders.setMainFrameBuffer(id);
}