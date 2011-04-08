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
   GLfloat color[] = { 0.3f, 0.3f, 0.7f, 1 };
   m_clearColor[0] = color[0];
   m_clearColor[1] = color[1];
   m_clearColor[2] = color[2];
   m_clearColor[3] = color[3];

   m_shaders = new msShaderPrograms();
}

void msScene::newSize(GLint width, GLint height)
{
	_width = width;
	_height = height;
	
	m_shaders->notifySizeChanged(width, height);
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
   freeResources();
}

//=================================================================================================================================
///
/// Loads all the data from the file (textures, attributes, uniforms, shaders...)
///
/// \param fileName is the name for the file where we get the data
///
/// \return bool saying whether we passed or failed
//=================================================================================================================================
bool msScene::loadData(const char* filename)
{
   m_shaders->loadFile(filename);
   return true;
}





//=================================================================================================================================
///
/// Deletes all the GL resources we have allocated
///
/// \param void
///
/// \return void
//=================================================================================================================================
void msScene::freeResources()
{
   delete m_shaders;
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

msParticleEmitter *pe1;
msParticleEmitter *pe2;
msParticleEmitter *pe3;

static const GLfloat g_fbVertexPositions[] = {
	-1.0f, -1.0f,  -1.0f,  1.0f,
	1.0f, -1.0f,  -1.0f,  1.0f,
	-1.0f,  1.0f,  -1.0f, 1.0f,
	1.0f,  1.0f,  -1.0f, 1.0f,
};

static const GLfloat g_fbVertexTexcoord[] = {
	0.0f, 0.f,   
	1.f,  0.f,    
	0.0f, 1.f,    	
	1.f,  1.f,   
};

static const GLuint g_fbIndices[] = {
	0, 1, 2, 3,
};


static const GLfloat g_vertexPositions[] = {
	-1.0f, -0.7f,  -1.0f,  1.0f,
	0.7f, -0.7f,  -1.0f,  1.0f,
	-0.7f,  0.7f,  -1.0f, 1.0f,
	0.5f,  0.5f,  -1.0f, 1.0f,
};

static const GLfloat g_vertexColors[] = {
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
};

static const GLfloat g_vertexTexcoord[] = {
	0.0f, 0.f,   
	1.f,  0.f,    
	0.0f, 1.f,    	
	1.f,  1.f,   
};

static const GLuint g_indices[] = {
	0, 1, 2, 3,
};

static const GLfloat prim[] =
{
	0.f, 0.f, -1.f, 1.0f,
	0.f, 1.f, -1.f, 1.0f,
	1.f, 0.f, -1.f, 1.0f,
	1.f, 1.f, -1.f, 1.0f,	
};

//=================================================================================================================================
///
/// Draws the current frame.
///
/// \param none
///
/// \return null
//=================================================================================================================================

float radius = -1.0f;
float power = 1.0f;
float ep[] = {0.0f, 0.0f};
bool animate = 0;

float radius_min = 0.0f;
float radius_max = 1500.0f;
float radius_step = 37.0f;

void msScene::Init()
{
	m_shaders->notifySizeChanged(_width, _height);

	pe1 = new msParticleEmitter(
		// explosion
		Vector2fMake(0.0f, 0.0f),//position:
		Vector2fMake(0.021f, 0.031f),//sourcePositionVariance:
		0.01f,//speed:
		0.007f,//speedVariance:
		1.0f,//particleLifeSpan:
		0.5f,//particleLifespanVariance:
		0.0f,//angle:
		360.0f,//angleVariance:
		Vector2fMake(0.0f, -0.00025f),//gravity:
		Color4fMake(1.0f, 0.5f, 0.05f, 1.0f),//startColor:
		Color4fMake(0.0f, 0.0f, 0.0f, 0.5f),//startColorVariance:
		Color4fMake(0.2f, 0.0f, 0.0f, 0.0f),//finishColor:
		Color4fMake(0.2f, 0.0f, 0.0f, 0.0f),//finishColorVariance:
		200,//maxParticles:
		190,//particleSize:
		30,//particleSizeVariance:
		-1,//0.125f,//duration:
		GL_TRUE//blendAdditive:
		);

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
		Color4fMake(0.5f, 0.5f, 0.5f, 1.0f),//startColor:
		Color4fMake(0.0f, 0.0f, 0.0f, 0.5f),//startColorVariance:
		Color4fMake(0.1f, 0.1f, 0.1f, 0.0f),//finishColor:
		Color4fMake(0.0f, 0.0f, 0.0f, 0.0f),//finishColorVariance:
		1000,//maxParticles:
		20,//particleSize:
		5,//particleSizeVariance:
		-1,//0.125f,//duration:
		GL_FALSE//blendAdditive:
		);

}
int c = 0;

void msScene::drawBackground()
{
	// render fire into texture using particle shaders
	msShaderProgram *program = m_shaders->getProgramByName("texture_aftershock");
	program->use();

	// Switch the render target to the current FBO to update the texture map
	program->getFrameBuffer("renderTex")->bind();

	// FBO attachment is complete?
	if (program->getFrameBuffer("renderTex")->isComplete())		
	{
		//int textureSize = max(this->_width, this->_height);

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
		glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, g_indices );

		program->getUniform("tex")->set1i(program->getTexture("ms0")->getUnit());
		program->getAttribute("position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, prim);

		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, g_indices);
	}

	// Unbind the FBO so rendering will return to the backbuffer.
	m_shaders->getMainFrameBuffer()->bind();

	// usual renderer

	// Set viewport to size of framebuffer and clear color and depth buffers

	// Bind updated texture map
	glActiveTexture(GL_TEXTURE0 + program->getFrameBuffer("renderTex")->getTexture()->getUnit());
	glBindTexture(GL_TEXTURE_2D, program->getFrameBuffer("renderTex")->getTexture()->getId());

	program->getUniform("tex")->set1i(program->getFrameBuffer("renderTex")->getTexture()->getUnit());
	program->getAttribute("position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, g_fbVertexPositions );
	program->getAttribute("texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, g_fbVertexTexcoord );

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, g_fbIndices );	



	if(animate)
	{
		program->getAttribute("radius")->set1f(radius);
		program->getAttribute("power")->set1f(power);
		program->getUniform("ep")->set2f(ep[0], ep[1]);

		c--;
		if(c < 40)
		{
			radius += radius_step;
			power -= radius_step / (radius_max - radius_min);
			if(radius > radius_max)
			{
				animate = 0;
				radius = -1.0f;
			}
		}		
	}
}



void msScene::drawExplosion()
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
		glViewport(0, 0, _width, _height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// render particles
		pe1->renderParticles(program);
		pe1->update(0.015f);

		//pe2->renderParticles(program);
		pe2->update(0.015f);
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
	particleCompleteProgram->getAttribute("a2_position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, g_fbVertexPositions );
	particleCompleteProgram->getAttribute("a2_texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, g_fbVertexTexcoord );

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, g_fbIndices );	

	glDisable(GL_BLEND);
}


void msScene::drawFrame()
{
	m_shaders->getMainFrameBuffer()->bind();

	glViewport(0, 0, _width, _height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	drawBackground();

	drawExplosion();	
}

void msScene::mouseClick(int x, int y)
{
	c = 50;
	animate = 1;
	radius = radius_min;
	power = 1.0f;
	ep[0] = (float)x;
	ep[1] = (float)this->_height - (float)y;

	pe1->active = true;
	pe1->duration = 0.125f;
	pe1->sourcePosition.x = (ep[0] / (float)this->_width * 2.0f) -1.0f;
	pe1->sourcePosition.y = (ep[1] / (float)this->_height * 2.0f) -1.0f;

	pe2->active = true;
	pe2->duration = 0.125f;
	pe2->sourcePosition.x = (ep[0] / (float)this->_width * 2.0f) -1.0f;
	pe2->sourcePosition.y = (ep[1] / (float)this->_height * 2.0f) -1.0f;

}