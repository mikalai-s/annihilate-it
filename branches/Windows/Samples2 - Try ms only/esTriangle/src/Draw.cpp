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

#include "Scene.h"
#include "..\ms\msParticleEmitter.h"
#include "Uniform.h"

#include "..\ms\ShaderProgram\msShaderPrograms.h"

msParticleEmitter *pe1;
msParticleEmitter *pe2;
msParticleEmitter *pe3;

static const float g_fbVertexPositions[] = {
	-1.0f, -1.0f,  -1.0f,  1.0f,
	1.0f, -1.0f,  -1.0f,  1.0f,
	-1.0f,  1.0f,  -1.0f, 1.0f,
	1.0f,  1.0f,  -1.0f, 1.0f,
};

static const float g_fbVertexTexcoord[] = {
	0.0f, 0.f,   
	1.f,  0.f,    
	0.0f, 1.f,    	
	1.f,  1.f,   
};

static const uint32 g_fbIndices[] = {
	0, 1, 2, 3,
};


static const float g_vertexPositions[] = {
	-1.0f, -0.7f,  -1.0f,  1.0f,
	0.7f, -0.7f,  -1.0f,  1.0f,
	-0.7f,  0.7f,  -1.0f, 1.0f,
	0.5f,  0.5f,  -1.0f, 1.0f,
};

static const float g_vertexColors[] = {
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
};

static const float g_vertexTexcoord[] = {
	0.0f, 0.f,   
	1.f,  0.f,    
	0.0f, 1.f,    	
	1.f,  1.f,   
};

static const uint32 g_indices[] = {
	0, 1, 2, 3,
};

static const float prim[] =
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
bool animate = FALSE;

float radius_min = 0.0f;
float radius_max = 1500.0f;
float radius_step = 37.0f;

GLuint _renderTextureId;
GLuint _renderTextureUnit = 10;
GLuint _fbo;

msShaderPrograms* shaders;

void Scene::Init()
{
	shaders = new msShaderPrograms();
	shaders->loadFile("./data/uniforms.txt");

	pe1 = new msParticleEmitter("texture.png",
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
/*
	// init framebuffer and texture to render to (for fire)

	// Generate handles for two dynamically rendered texture maps
	glGenTextures(1, &_renderTextureId);

	//glActiveTexture(GL_TEXTURE_2D + _renderTextureUnit);

	int textureSize = max(this->_width, this->_height);

	// Bind and configure the texture
	glActiveTexture(GL_TEXTURE0 + _renderTextureUnit);
	glBindTexture(GL_TEXTURE_2D, _renderTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize, textureSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Generate handles for two Frame Buffer Objects
	glGenFramebuffers(1, &_fbo);

	// Attach the texture to the first color buffer of an FBO and clear it
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTextureId, 0);
	//glClear(GL_COLOR_BUFFER_BIT);

	// don't forget release them:
	/*
	// Delete the FBOs
	glDeleteFramebuffers(2, m_hFBO);

	// Delete the textures
	glDeleteTextures(2, m_hTexture);
	*/
}


void Scene::drawBackground()
{
	shaders->getMainFrameBuffer()->bind();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT );

	msShaderProgram *program = shaders->getProgramByName("texture_aftershock");
	program->use();

	program->getUniform("tex")->set1i(program->getTexture("tex0")->getUnit());
	program->getAttribute("position")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, g_vertexPositions );
	program->getAttribute("color")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, g_vertexColors );
	program->getAttribute("texcoord")->setPointerAndEnable( 2, GL_FLOAT, 0, 0, g_vertexTexcoord );

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, g_indices );

	program->getUniform("tex")->set1i(program->getTexture("ms0")->getUnit());
	program->getAttribute("position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, prim);

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, g_indices);

	program->getAttribute("radius")->set1f(radius);
	program->getAttribute("power")->set1f(power);
	program->getUniform("ep")->set2f(ep[0], ep[1]);

	if(animate)
	{
		radius += radius_step;
		power -= radius_step / (radius_max - radius_min);
		if(radius > radius_max)
		{
			animate = FALSE;
			radius = -1.0f;
		}
	}
}

void Scene::drawExplosion()
{
	// render fire into texture using particle shaders
	msShaderProgram *program = shaders->getProgramByName("particle_create");
	program->use();

	// Switch the render target to the current FBO to update the texture map
	program->getFrameBuffer("renderTex")->bind();

	// FBO attachment is complete?
	if (program->getFrameBuffer("renderTex")->isComplete())		
	{
		int textureSize = max(this->_width, this->_height);

		// Set viewport to size of texture map and erase previous image
		glViewport(0, 0, textureSize, textureSize);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// render particles
		pe1->renderParticles(program);
		pe1->update(0.015f);
	}

	// Unbind the FBO so rendering will return to the backbuffer.
	shaders->getMainFrameBuffer()->bind();

	// usual renderer

	// Set viewport to size of framebuffer and clear color and depth buffers
	glViewport(0, 0, _width, _height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind updated texture map
	glBindTexture(GL_TEXTURE_2D, program->getFrameBuffer("renderTex")->getTexture()->getId());

	msShaderProgram *particleCompleteProgram = shaders->getProgramByName("particle_complete");
	particleCompleteProgram->use();

	particleCompleteProgram->getUniform("u2_texture")->set1i(program->getFrameBuffer("renderTex")->getTexture()->getUnit());
	particleCompleteProgram->getAttribute("a2_position")->setPointerAndEnable(4, GL_FLOAT, 0, 0, g_fbVertexPositions );
	particleCompleteProgram->getAttribute("a2_texcoord")->setPointerAndEnable(2, GL_FLOAT, 0, 0, g_fbVertexTexcoord );

	// draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
	glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, g_fbIndices );	

	glDisable(GL_BLEND);
}


void Scene::drawFrame()
{
	drawBackground();

	drawExplosion();	
}

void Scene::mouseClick(int x, int y)
{
	animate = TRUE;
	radius = radius_min;
	power = 1.0f;
	ep[0] = (float)x;
	ep[1] = (float)this->_height - (float)y;

	pe1->active = true;
	pe1->duration = 0.125f;
	pe1->sourcePosition.x = (ep[0] / (float)this->_width * 2.0f) -1.0f;
	pe1->sourcePosition.y = (ep[1] / (float)this->_height * 2.0f) -1.0f;

}