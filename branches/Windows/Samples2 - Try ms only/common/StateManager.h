//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Definitions for the StateManager class.  This class saves off and restores OpenGL state.  The state saved off pertains to
// rendering funtionality.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/StateManager.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "ArgDefines.h"

class StateManager
{
public:
   /// \brief This saves the current OpenGL state
   void SaveState();
   /// \brief This restores the OpenGL state from the last save
   void RestoreState();
private:

   // All the data below are shadow copies of OpenGL state of the same name
   int32 m_currentProgram;
   int32 m_activeTexture;
   int32 m_textureBinding0_2d;
   uint8 m_bCulling;
   uint8 m_bScissorTest;
   uint8 m_bBlend;
   int32 m_blendSrcRgb;
   int32 m_blendSrcAlpha;
   int32 m_blendDstRgb;
   int32 m_blendDstAlpha;
   int32 m_blendEquationRgb;
   int32 m_blendEquationAlpha;
   float32 m_blendColor[4];
   uint8 m_bColorWriteMask[4];
   uint8 m_bDepthWriteMask;
   int32 m_bStencilWriteMask;
   uint8 m_bDepthTest;
   uint8 m_bStencilTest;
   uint8 m_bSampleAlphaToCoverage;
   uint8 m_bSampleCoverage;
   int32 m_elementArrayBufferBinding;
   int32 m_arrayBufferBinding;
   int32 m_vertexAttribArrayEnabled[4];
   int32 m_vertexAttribArraySize[4];
   int32 m_vertexAttribArrayStride[4];
   int32 m_vertexAttribArrayType[4];
   int32 m_vertexAttribArrayNormalized[4];
   void* m_vertexAttribArrayPointer[4]; 
};

