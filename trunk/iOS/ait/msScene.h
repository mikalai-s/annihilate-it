//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Definitions for the Scene class.  This class wraps in drawing in GL.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Scene.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#pragma once

#include "msGL.h"

#include <iostream>
#include <fstream>

#include "ShaderProgram/msShaderPrograms.h"
#include "msParticleEmitter.h"
#include "Annihilate/msPalette.h"
#include "Annihilate/msBoxGrid.h"
#include "Annihilate/msBoxGridRenderer.h"





//=================================================================================================================================
/// Scene class - This class loads and draws the scene.
//=================================================================================================================================
class msScene
{
private:

    GLfloat afterShockRadius;
    GLfloat afterShockPower;
    GLfloat afterShockLocation[2];
    GLboolean animate;

    GLfloat afterShockRadiusMin;
    GLfloat afterShockRadiusMax;
    GLfloat afterShockRadiusStep;


    msPalette *palette;
    msBoxGrid *boxGrid;
    msBoxGridRenderer *renderer;    
    
    /// Contains all the uniform/texture/attribute/shader data
    msShaderPrograms shaders;
    
    /// Pointer to the clear color
    GLfloat clearColor[4];
    
    msSizef size;


public:
   /// \brief Constructor.
   msScene();

   /// \brief Destructor.
   ~msScene();

   void init();

   /// \brief Draws the frame.
   void drawFrame();

   /// \brief Loads all the data from the file (textures, attributes, uniforms, shaders...)
   bool loadData(string filename);

   /// \brief Puts the named program in use
   void useProgram( const char* name );

   /// \brief Updates the value of a uniform
   bool updateUniform( const char* name, GLfloat* vals );

   /// \brief Binds a texture into GL
   bool bindTexture( const char* name, GLint width, GLint height );

   /// \brief Binds an FBO into GL
   bool bindFbo( const char* name, GLint width, GLint height );

   void mouseClick(int x, int y, int direction);

   void newSize(int width, int height);

   void drawBackground();
   void drawExplosion();

   void setMainFrameBuffer(GLint id);

   void undoLastMove();

   void start();

   void end();
};


