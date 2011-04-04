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

#include "Scene.h"

using namespace std;

//=================================================================================================================================
///
/// Constructor
///
/// \param none
///
/// \return null
//=================================================================================================================================
Scene::Scene()
{
   float32 color[] = { 0.3f, 0.3f, 0.7f, 1 };
   m_clearColor[0] = color[0];
   m_clearColor[1] = color[1];
   m_clearColor[2] = color[2];
   m_clearColor[3] = color[3];
}

void Scene::newSize(int width, int height)
{
	_width = width;
	_height = height;
}

//=================================================================================================================================
///
/// Destructor
///
/// \param void
///
/// \return void
//=================================================================================================================================
Scene::~Scene()
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
bool Scene::loadData(const char8* filename)
{
   if ( ! m_uniforms.parseUniforms( filename ) )
   {
      cout << "Failed to parse data file." << endl;
      m_clearColor[0] = 1;
      m_clearColor[1] = 0;
      m_clearColor[2] = 0;
      m_clearColor[3] = 1;
      return false;
   }
   return true;
}

//=================================================================================================================================
///
/// Puts the named program in use
///
/// \param name - The name we gave to the program
///
/// \return void
//=================================================================================================================================
void Scene::useProgram( const char8* name )
{
   GLuint progHandle = m_uniforms.getProgramHandle( name );
   glUseProgram( progHandle );
}

//=================================================================================================================================
///
/// Updates the value of a uniform
///
/// \param name - The name we gave to the uniform
/// \param vals - An array of values we want to to replace the current uniform values with
///
/// \return true=pass ... false=fail
//=================================================================================================================================
bool Scene::updateUniform( const char8* name, float32* vals )
{
   return m_uniforms.updateOneUniform( name, vals );
}

//=================================================================================================================================
///
/// Deletes all the GL resources we have allocated
///
/// \param void
///
/// \return void
//=================================================================================================================================
void Scene::freeResources()
{
   m_uniforms.freeAllData();
}

//=================================================================================================================================
///
/// Binds a texture into GL
///
/// \param name - The name we gave the texture
/// \param width - The width of the texture
/// \param height - The height of the texture
///
/// \return true = pass     false = fail
//=================================================================================================================================
bool Scene::bindTexture( const char8* name, int32 width, int32 height )
{
   bool ret;
   ret = m_uniforms.bindTexture( name, width, height );
   return ret;
}

//=================================================================================================================================
///
/// Binds an FBO into GL
///
/// \param name - The name we gave the fbo
/// \param width - The width of the fbo
/// \param height - The height of the fbo
///
/// \return true = pass     false = fail
//=================================================================================================================================
bool Scene::bindFbo( const char8* name, int32 width, int32 height )
{
   bool ret;
   ret = m_uniforms.bindFbo( name, width, height );
   return ret;
}
