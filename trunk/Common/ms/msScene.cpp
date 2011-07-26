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
   this->clearColor[0] = color[0];
   this->clearColor[1] = color[1];
   this->clearColor[2] = color[2];
   this->clearColor[3] = color[3];

   this->afterShockRadius = -1.0f;
   this->afterShockPower = 1.0f;
   this->afterShockLocation[0] = 0.0f;
   this->afterShockLocation[1] = 0.0f;
   this->animate = 0;

   this->afterShockRadiusMin = 0.0f;
   this->afterShockRadiusMax = 1500.0f;
   this->afterShockRadiusStep = 37.0f;

   this->palette = 0;
   this->boxGrid = 0;
    this->renderer = 0;
}

void msScene::newSize(GLint width, GLint height)
{
    this->size.width = width;
    this->size.height = height;
    
    this->shaders.notifySizeChanged(width, height);    
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
    if(this->palette != 0)
        delete this->palette;
    if(this->boxGrid != 0)
        delete this->boxGrid;
    if(this->renderer != 0)
        delete this->renderer;
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
   this->shaders.loadFile(filename);
    this->renderer = new msBoxGridRenderer(&this->shaders, 0);

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
    this->shaders.notifySizeChanged(this->size.width, this->size.height);

    // init palette
    this->palette = new msPalette(colorMap, 8);    
}


void msScene::drawFrame()
{
    //if(this->renderer != 0)
        this->renderer->draw(this->size);
}

void msScene::mouseClick(int x, int y, int direction)
{
    if(this->boxGrid == 0 || this->renderer == 0)
        return;

    msPoint3f touchPoint;
    touchPoint.x = ((GLfloat)x / (GLfloat)this->size.width);
    touchPoint.y = ((GLfloat)y / (GLfloat)this->size.height);
    
    this->boxGrid->setDirection(direction);
    this->boxGrid->removeSimilarItemsAtPoint(touchPoint);
}

void msScene::setMainFrameBuffer(GLint id)
{
    this->shaders.setMainFrameBuffer(id);
}

void msScene::undoLastMove()
{
    if(this->boxGrid != 0)
        this->boxGrid->undo();
}

void msScene::start()
{
#define NUM_ROWS 10
#define NUM_COLS 8

    msBoxFaceData backFaces[NUM_ROWS * NUM_COLS];

    memset(backFaces, 0, sizeof(backFaces));

    if(this->boxGrid != 0)
    {
        this->boxGrid->extractPattern(backFaces);        

        delete this->boxGrid;
     }
    
    GLint pattern[NUM_ROWS * NUM_COLS] = 
    {
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
    };
    
   //this->boxGrid = new msBoxGrid(this->palette, pattern, NUM_ROWS, NUM_COLS, 1.0f, 1.0f);

    this->boxGrid = new msBoxGrid(this->palette, 4, NUM_ROWS, NUM_COLS, 1.0f, 1.0f);
    this->renderer->setBoxGrid(this->boxGrid);
    
    this->boxGrid->setBackPattern(backFaces);
    this->boxGrid->show();
}

void msScene::end()
{
    //this->boxGrid->hide();

    delete this->boxGrid;

    this->boxGrid = 0;

    
}