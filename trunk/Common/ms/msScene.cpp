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

#include "Annihilate/msSpline.h"
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
	{0.851f, 0.851f, 0.851f, 1.0f}, // gray	
	{1.000f, 0.412f, 0.337f, 1.0f}, // red
	{0.467f, 0.357f, 1.000f, 1.0f}, // blue
	{0.502f, 0.824f, 0.776f, 1.0f}, // green	
	{0.714f, 1.000f, 0.608f, 1.0f}, // lime
	{0.843f, 0.420f, 1.000f, 1.0f}, // violet
	{0.478f, 0.824f, 1.000f, 1.0f}, // turquoise
	{1.000f, 0.757f, 0.369f, 1.0f}, // yellow
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

#define NUM_ROWS 20
#define NUM_COLS 20
	
    GLint pattern[NUM_ROWS * NUM_COLS] = 
    {
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
        //1, 2, 3, 4, 5, 6, 7, 2, 1, 4,
		//1,1,
	//	1,2,
    };
	
	//m_boxGrid = new msBoxGrid(m_palette, pattern, NUM_ROWS, NUM_COLS, 1.0f, 1.0f);
	
	m_boxGrid = new msBoxGrid(m_palette, 4, NUM_ROWS, NUM_COLS, 1.0f, 1.0f);
	
	m_renderer = new msBoxGridRenderer(&m_shaders);
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

	return;
}

int getShiftDirection()
{
	return MS_BOX_SHIFT_DOWN;
}

void msScene::mouseClick(int x, int y)
{
	msPointf touchPoint;
	touchPoint.x = ((GLfloat)x / (GLfloat)_width);
	touchPoint.y = ((GLfloat)y / (GLfloat)_height);
	
	m_boxGrid->removeSimilarItemsAtPoint(touchPoint);
}

void msScene::setMainFrameBuffer(GLint id)
{
	m_shaders.setMainFrameBuffer(id);
}

void msScene::undoLastMove()
{
	m_boxGrid->undo();
}
