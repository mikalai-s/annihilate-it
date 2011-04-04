//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation for the UniformData class.  This is a base class for the Uniform class.  This holds all the data.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/UniformData.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "UniformData.h"

//=================================================================================================================================
//=================================================================================================================================
//  Constructor(s) / Destructor(s) Block
//=================================================================================================================================
//=================================================================================================================================

//=================================================================================================================================
///
/// Constructor
///
/// \param none
///
/// \return null
//=================================================================================================================================
UniformData::UniformData() :
   m_location(0),
   m_name(NULL),
   m_type(0),
   m_textureHandle(0),
   m_textureUnit(0),
   m_depthTex(false),
   m_width(0),
   m_height(0),
   m_fboHandle(0),
   m_colorRbHandle(0),
   m_vertShaderHandle(0),
   m_fragShaderHandle(0),
   m_progHandle(0),
   m_attachedProgram(0)   
{
}

//=================================================================================================================================
///
/// Destructor
///
/// \param none
///
/// \return null
//=================================================================================================================================
UniformData::~UniformData()   
{
   if ( m_name != NULL )
   {
      delete [] m_name;
      m_name = NULL;
   }
}




