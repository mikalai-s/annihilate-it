//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation of the StateManager class.  This class saves off and restores OpenGL state.  The state saved off pertains to
// rendering funtionality.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/StateManager.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "StateManager.h"
#include <GLES2/gl2.h>

//=================================================================================================================================
///
/// SaveState - This saves the current OpenGL state
///
/// \param void
///
/// \return void
//=================================================================================================================================
void StateManager::SaveState()
{
   glGetIntegerv( GL_CURRENT_PROGRAM, &m_currentProgram );
   glGetIntegerv( GL_ACTIVE_TEXTURE, &m_activeTexture );
   // Currently we only care about tex unit 0 2d, might want to add more units here later
   glActiveTexture( GL_TEXTURE0 );
   glGetIntegerv( GL_TEXTURE_BINDING_2D, &m_textureBinding0_2d );
   glActiveTexture( m_activeTexture );
   
   m_bCulling = glIsEnabled( GL_CULL_FACE );
   m_bScissorTest = glIsEnabled( GL_SCISSOR_TEST );
   m_bBlend = glIsEnabled( GL_BLEND );
   glGetIntegerv( GL_BLEND_SRC_RGB, &m_blendSrcRgb );
   glGetIntegerv( GL_BLEND_SRC_ALPHA, &m_blendSrcAlpha );
   glGetIntegerv( GL_BLEND_DST_RGB, &m_blendDstRgb );
   glGetIntegerv( GL_BLEND_DST_ALPHA, &m_blendDstAlpha );
   glGetIntegerv( GL_BLEND_EQUATION_RGB, &m_blendEquationRgb );
   glGetIntegerv( GL_BLEND_EQUATION_ALPHA, &m_blendEquationAlpha );
   glGetFloatv( GL_BLEND_COLOR, m_blendColor );
   glGetBooleanv( GL_COLOR_WRITEMASK, (GLboolean*)&m_bColorWriteMask );
   glGetBooleanv( GL_DEPTH_WRITEMASK, (GLboolean*)&m_bDepthWriteMask );
   glGetBooleanv( GL_STENCIL_WRITEMASK, (GLboolean*)&m_bStencilWriteMask );
   m_bDepthTest = glIsEnabled( GL_DEPTH_TEST );
   m_bStencilTest = glIsEnabled( GL_STENCIL_TEST );
   m_bSampleAlphaToCoverage = glIsEnabled( GL_SAMPLE_ALPHA_TO_COVERAGE );
   m_bSampleCoverage = glIsEnabled( GL_SAMPLE_COVERAGE );
   glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &m_elementArrayBufferBinding );
   glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &m_arrayBufferBinding );
   for( int32 i = 0; i < 4; ++i )
   {
      glGetVertexAttribiv( i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &(m_vertexAttribArrayEnabled[i]) );
      glGetVertexAttribiv( i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &(m_vertexAttribArraySize[i]) );
      glGetVertexAttribiv( i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &(m_vertexAttribArrayStride[i]) );
      glGetVertexAttribiv( i, GL_VERTEX_ATTRIB_ARRAY_TYPE, &(m_vertexAttribArrayType[i]) );
      glGetVertexAttribiv( i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &(m_vertexAttribArrayNormalized[i]) );
      glGetVertexAttribPointerv( i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &(m_vertexAttribArrayPointer[i]) );
   }

}

// Just a helper function to make enabling or disabling cleaner
void OglEnableDisable( GLenum def, uint8 enable )
{
   if ( enable )
   {
      glEnable( def );

   }
   else
   {
      glDisable( def );
   }
}

//=================================================================================================================================
///
/// RestoreState - This restores the OpenGL state from the last save
///
/// \param void
///
/// \return void
//=================================================================================================================================
void StateManager::RestoreState()
{
   glUseProgram( m_currentProgram );

   // Currently we only care about tex unit 0 2d, might want to add more units here later
   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_textureBinding0_2d );
   glActiveTexture( m_activeTexture );

   OglEnableDisable( GL_CULL_FACE, m_bCulling );
   OglEnableDisable( GL_SCISSOR_TEST, m_bScissorTest );
   OglEnableDisable( GL_BLEND, m_bBlend );

   glBlendFuncSeparate( m_blendSrcRgb, m_blendDstRgb, m_blendSrcAlpha, m_blendDstAlpha );
   glBlendEquationSeparate( m_blendEquationRgb, m_blendEquationAlpha );
   glBlendColor( m_blendColor[0], m_blendColor[1], m_blendColor[2], m_blendColor[3] );
   glColorMask( m_bColorWriteMask[0], m_bColorWriteMask[1], m_bColorWriteMask[2], m_bColorWriteMask[3] );
   glDepthMask( m_bDepthWriteMask );
   glStencilMask( m_bStencilWriteMask );

   OglEnableDisable( GL_DEPTH_TEST, m_bDepthTest );
   OglEnableDisable( GL_STENCIL_TEST, m_bStencilTest );
   OglEnableDisable( GL_SAMPLE_ALPHA_TO_COVERAGE, m_bSampleAlphaToCoverage );
   OglEnableDisable( GL_SAMPLE_COVERAGE, m_bSampleCoverage );

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elementArrayBufferBinding );
   glBindBuffer( GL_ARRAY_BUFFER, m_arrayBufferBinding );
   
   for( int32 i = 0; i < 4; ++i )
   {
      if ( m_vertexAttribArrayEnabled[i] )
      {
         glEnableVertexAttribArray( i );
      }
      else
      {
         glDisableVertexAttribArray( i );
      }

      glVertexAttribPointer( i , m_vertexAttribArraySize[i], m_vertexAttribArrayType[i], m_vertexAttribArrayNormalized[i],
                             m_vertexAttribArrayStride[i], m_vertexAttribArrayPointer[i] );
   }
}

