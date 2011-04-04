//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Definitions for the Font class.  This class is used to render a font in OpenGL ES.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Font.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#ifndef _FONT_H_
#define _FONT_H_

#include "ArgDefines.h"

// Simple structure that contains all the data we need to know about characters in the font texture and
// how to place them in OpenGL ES.
struct CharContainer
{
   float32 minS;
   float32 minT;
   float32 maxS;
   float32 maxT;
   float32 minX;
   float32 minY;
   float32 maxX;
   float32 maxY;
   float32 xAdvance;
};

class Font
{
public:

   /// \brief Constructor
   Font();
   
   /// \brief Destructor
   ~Font();

   /// \brief Initializes a font texture (color and magnification size specified at this time)
   void Init( float32 r, float32 g, float32 b, float32 a, float32 magFactor, 
              int32* pFontHeader, int32* pCharData, char8* pFontImageData, float32 winWidth, float32 winHeight );

   /// \brief Deletes all the font data
   void Delete();

   /// \brief Prints the string into OpenGL ES
   void Print( const char8* str, float32 x, float32 y );

   /// \brief This parses through raw character data and puts it into an efficent format for this class
   void ResizeCharData( int32 *charData, float32 winWidth, float32 winHeight, float32 magFactor );

private:
   /// \brief Intelligently grows the size of all the vertex buffers needed to render the font.
   void SmartGrowMaxCharBufSize( int32 newSize );

   /// \brief Updates all the vertex buffers with the data needed to render this font
   void UpdateVertexBuffers( const char8 *str, float32 x, float32 y );

   /// Contains all the data we need to render each character
   CharContainer m_charData[256];

   /// The OpenGL vertex shader id
   uint32 m_vertShaderHandle;

   /// The OpenGL fragment shader id
   uint32 m_fragShaderHandle;

   /// The OpenGL program id
   uint32 m_progHandle;

   /// The OpenGL texture id
   uint32 m_texId;

   /// The line height of the charaters
   float32 m_lineHeight;

   /// The font texture height
   float32 m_texWidth;

   /// The font texture width
   float32 m_texHeight;

   // The number of characters stored in the raw char data
   int32 m_numCharData;

   /// Location of the position attribute array (OpenGL)
   int32 m_posLocAttrib;

   /// Location of the texture coordinate attribute array (OpenGL)
   int32 m_txCoordLocAttrib;

   /// Location of the font texture uniform (ie texture unit number) (OpenGL)
   int32 m_fontTexLoc;

   /// Location of the color uniform (OpenGL)
   int32 m_colLoc;

   /// Keeps track of how many characters we can handle based on the size of our buffers
   int32 m_maxCharBufSize;

   /// System memory buffer containing the position of all the characters in a string
   float32 *m_systemPosBuffer;

   /// System memory buffer containing the texture coords of all the characters in a string
   float32 *m_systemTexBuffer;

   /// OpenGL index buffer id
   uint32 m_indBufferHandle;

   /// OpenGL vertex buffer id for position
   uint32 m_posBufferHandle;

   /// OpenGL vertex buffer id for texture coords
   uint32 m_texBufferHandle;
};

#endif // _FONT_H_

