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