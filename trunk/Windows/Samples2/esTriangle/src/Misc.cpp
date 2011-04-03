//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Various helpful functions that are project specific.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/esTriangle/src/Misc.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "Misc.h"
#include "assert.h"

// Global holding all the global egl data
EglStruct g_egl;

//=================================================================================================================================
///
/// Builds the egl attrib list
///
/// \param attribList - The egl attrib list
///
/// \return void
//=================================================================================================================================
void BuildAttribList( EGLint *attribList )
{
   int    nAttribCount = 0;
   
   attribList[nAttribCount++] = EGL_RED_SIZE;
   attribList[nAttribCount++] = 5; 
   attribList[nAttribCount++] = EGL_GREEN_SIZE;
   attribList[nAttribCount++] = 6;
   attribList[nAttribCount++] = EGL_BLUE_SIZE;
   attribList[nAttribCount++] = 5;
   attribList[nAttribCount++] = EGL_ALPHA_SIZE;
   attribList[nAttribCount++] = 0;
   attribList[nAttribCount++] = EGL_DEPTH_SIZE;
   attribList[nAttribCount++] = 16;
   attribList[nAttribCount++] = EGL_STENCIL_SIZE;
   attribList[nAttribCount++] = 0;
   attribList[nAttribCount++] = EGL_SAMPLES;
   attribList[nAttribCount++] = 4;
   attribList[nAttribCount++] = EGL_NONE;
   
   assert( nAttribCount < MAX_EGL_ATTRIBUTES );
}

//=================================================================================================================================
///
/// Does any special stuff we need at init time
///
/// \param none
///
/// \return if we return false the app exits
//=================================================================================================================================
void MainFuncInit()
{

}

//=================================================================================================================================
///
/// Sets up the textures including compression (located here for convienience.
///
/// \param filename - The file name of the texture to be loaded
/// \param texUnit - The texture unit we want to use for this texture
///
/// \return - The ES texture bind number
//=================================================================================================================================
GLuint LoadCompressedTexture( const char8 *filename, const char8 *sType, GLuint texUnit )
{
   assert( 0 );
   return 0;
}