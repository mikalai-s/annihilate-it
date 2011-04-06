//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Definitions for the Uniform class.  This class loads textures, attributes, and uniforms.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/esTriangle/src/Misc.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#ifndef _MISC_H_
#define _MISC_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#define MAX_EGL_ATTRIBUTES 64

struct EglStruct
{
   EGLDisplay  dsp;
   EGLConfig   cfg;
   EGLContext  cxt;
   EGLSurface  surf;
};

extern EglStruct g_egl;

/// \brief Builds the egl attrib list
void BuildAttribList( EGLint *attribList );

/// \brief Does any special stuff we need at init time
void MainFuncInit();


#endif // _MISC_H_

