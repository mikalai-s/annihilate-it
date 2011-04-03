//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation of the Font class.  This class is used to render a font in OpenGL ES.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Font.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "StateManager.h"  // This class saves and restores state
#include "Font.h"
#include <GLES2/gl2.h>
#include <assert.h>
#include <fstream>
#include <sstream>

using namespace std;

// This is the fragment shader we will use to render the font
const char8* FontFragShader[1] = 
{" precision mediump float;\n\
   uniform sampler2D fontTex;\n\
   varying vec4 texCoord0;\n\
   varying vec4 fontColor;\n\
   void main()\n\
   {\n\
      vec4 temp = fontColor * texture2D(fontTex, texCoord0.xy);\n\
      temp.a = temp.r + temp.g + temp.b;\n\
      gl_FragColor = temp;\n\
   }\n\
"};

// This is the vertex shader we will use to render the font
const char8* FontVertShader[1] =
{" attribute vec4 pos;\n\
   attribute vec4 txCoord;\n\
   uniform vec4 col;\n\
\n\
   varying vec4 texCoord0;\n\
   varying vec4 fontColor;\n\
void main()\n\
{\n\
    gl_Position = pos;\n\
    fontColor = col;\n\
    texCoord0 = txCoord;\n\
}\n\
"};

//=================================================================================================================================
///
/// Constructor
///
/// \param void
///
/// \return void
//=================================================================================================================================
Font::Font() :
   m_vertShaderHandle(0),
   m_fragShaderHandle(0),
   m_progHandle(0),
   m_texId(0),
   m_lineHeight(0),
   m_posLocAttrib(-1),
   m_txCoordLocAttrib(-1),
   m_fontTexLoc(-1),
   m_colLoc(-1),
   m_maxCharBufSize(0),
   m_systemPosBuffer(NULL),
   m_systemTexBuffer(NULL),
   m_indBufferHandle(0),
   m_posBufferHandle(0),
   m_texBufferHandle(0)
{
}

//=================================================================================================================================
///
/// Destructor
///
/// \param void
///
/// \return void
//=================================================================================================================================
Font::~Font()
{
   // I hope you called Delete().
}

//=================================================================================================================================
///
/// Init - Initializes a font texture (color and magnification size specified at this time)
///
/// \param r - The red color for the font
/// \param g - The green color for the font
/// \param b - The blue color for the font
/// \param a - The alpha color for the font
/// \param magFactor - The magnification you want to apply to the font (<1.0 is smaller >1.0 is bigger)
/// \param pFontHeader - Global information about the font texture
/// \param pCharData - Data about each character in the font
/// \param pFontImageData - The pixel values for the font texture
/// \param winWidth - The width of the window
/// \param winHeight - The height of the window
///
/// \return void
//=================================================================================================================================
void Font::Init( float32 r, float32 g, float32 b, float32 a, float32 magFactor,
                 int32* pFontHeader, int32* pCharData, char8* pFontImageData, float32 winWidth, float32 winHeight )
{
   StateManager sm;
   sm.SaveState();  // Save off the current OpenGL state

   // Delete old font if making a new one and one already exists
   Delete();

   SmartGrowMaxCharBufSize(128);

   if ( pFontHeader == NULL || pCharData == NULL || pFontImageData == NULL )
   {
      assert(0);
      return;
   }

   m_vertShaderHandle = glCreateShader( GL_VERTEX_SHADER );
   m_fragShaderHandle = glCreateShader( GL_FRAGMENT_SHADER );
   m_progHandle = glCreateProgram();

   glShaderSource( m_vertShaderHandle, 1, FontVertShader, 0 );
   glShaderSource( m_fragShaderHandle, 1, FontFragShader, 0 );
   glCompileShader(m_vertShaderHandle);
   glCompileShader(m_fragShaderHandle);

   glAttachShader( m_progHandle, m_vertShaderHandle );
   glAttachShader( m_progHandle, m_fragShaderHandle );
   glLinkProgram( m_progHandle );

   m_fontTexLoc = glGetUniformLocation(m_progHandle, "fontTex");
   m_colLoc = glGetUniformLocation(m_progHandle, "col");

   m_posLocAttrib = glGetAttribLocation(m_progHandle, "pos");
   m_txCoordLocAttrib = glGetAttribLocation(m_progHandle, "txCoord");

   glUseProgram( m_progHandle );
   glUniform4f( m_colLoc, r, g, b, a );

   // Data format is -- texWidth, texHeight, channels, bitsPerChannel, lineHeight, numCharData
   m_texWidth = (float32)pFontHeader[0];
   m_texHeight = (float32)pFontHeader[1];
   
   int32 texChannels = pFontHeader[2];
   int32 texBpc = pFontHeader[3];
   
   m_lineHeight = (float32)pFontHeader[4];
   m_numCharData = pFontHeader[5];

   ResizeCharData( pCharData, winWidth, winHeight, magFactor );

   glGenTextures( 1, &m_texId );
   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_texId );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

   if ( texChannels == 1 )
   {
      glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, (int32)m_texWidth, (int32)m_texHeight,
                    0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pFontImageData );
   }
   else
   {
      assert( 0);
   }
   sm.RestoreState(); // Restore the OpenGL state
}

//=================================================================================================================================
///
/// Delete - Deletes all the font related data
///
/// \param void
///
/// \return void
//=================================================================================================================================
void Font::Delete()
{
   if ( m_systemPosBuffer || m_systemTexBuffer )
   {
      glDeleteProgram( m_progHandle );
      glDeleteShader( m_vertShaderHandle );
      glDeleteShader( m_fragShaderHandle );
      glDeleteTextures( 1, &m_texId );

      delete [] m_systemPosBuffer;
      delete [] m_systemTexBuffer;

      glDeleteBuffers( 1, &m_indBufferHandle );
      glDeleteBuffers( 1, &m_posBufferHandle );
      glDeleteBuffers( 1, &m_texBufferHandle );

      m_systemPosBuffer = 0; 
      m_systemTexBuffer = 0;
   }
}

//=================================================================================================================================
///
/// Print - Prints a string to an OpenGL window
///
/// \param str - The string that we want to print
/// \param x - X position of the string (0 is left side of the window, 1 is the right side of the window)
/// \param y - Y position of the string (0 is the bottom of the window, 1 is the top of the window)
///
/// \return void
//=================================================================================================================================
void Font::Print( const char8* str, float32 x, float32 y )
{
   int32 stringLength = int32(strlen( str ));

   StateManager sm;
   sm.SaveState(); // Save the current OpenGL state

   glUseProgram( m_progHandle );
   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_texId );

   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

   glDisable( GL_CULL_FACE );
   glDisable( GL_SCISSOR_TEST );

   glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
   glDepthMask( GL_FALSE );
   glStencilMask( GL_FALSE );

   glDisable( GL_DEPTH_TEST );
   glDisable( GL_STENCIL_TEST );
   glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE );
   glDisable( GL_SAMPLE_COVERAGE );

   UpdateVertexBuffers( str, x, y );

   glBindBuffer( GL_ARRAY_BUFFER, m_posBufferHandle );
   glVertexAttribPointer( m_posLocAttrib, 2, GL_FLOAT, 0, 0, NULL );
   glEnableVertexAttribArray( m_posLocAttrib );

   glBindBuffer( GL_ARRAY_BUFFER, m_texBufferHandle );
   glVertexAttribPointer( m_txCoordLocAttrib, 2, GL_FLOAT, 0, 0,NULL );
   glEnableVertexAttribArray( m_txCoordLocAttrib );

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_indBufferHandle );
   glDrawElements( GL_TRIANGLES, stringLength * 6, GL_UNSIGNED_SHORT, NULL );
   
   sm.RestoreState(); // Restore the OpenGL state
}

//=================================================================================================================================
///
/// ResizeCharData - This parses through raw character data and puts it into an efficent format for this class
///
/// \param charData - The passed in raw data about the characters
/// \param winWidth - The window width
/// \param winHeight - The window height
/// \param magFactor - The magnification you want to apply to the font (<1.0 is smaller >1.0 is bigger)
///
/// \return void
//=================================================================================================================================
void Font::ResizeCharData( int32 *charData, float32 winWidth, float32 winHeight, float32 magFactor )
{
   memset( m_charData, 0, sizeof(m_charData) );

   // Format of data is: id, x, y, width, height, xoffset, yoffset, xadvance
   for( int32 j = 0; j < m_numCharData; ++j )
   {
      int32 id =        charData[ j*8 + 0 ];
      int32 x =         charData[ j*8 + 1 ];
      int32 y =         charData[ j*8 + 2 ];
      int32 width =     charData[ j*8 + 3 ];
      int32 height =    charData[ j*8 + 4 ];
      int32 xoffset =   charData[ j*8 + 5 ];
      int32 yoffset =   charData[ j*8 + 6 ];
      int32 xadvance =  charData[ j*8 + 7 ];

      m_charData[id].minS = x / m_texWidth;
      m_charData[id].minT = (m_texHeight - height - y) / m_texHeight;
      m_charData[id].maxS = (x + width) / m_texWidth;
      m_charData[id].maxT = (m_texHeight  - y) / m_texHeight;
      m_charData[id].minX = xoffset / winWidth * magFactor;
      m_charData[id].minY = (m_lineHeight - yoffset - height) / winHeight * magFactor;
      m_charData[id].maxX = (xoffset + width) / winWidth * magFactor;
      m_charData[id].maxY = (m_lineHeight-yoffset) / winHeight * magFactor;
      m_charData[id].xAdvance = xadvance * magFactor / winWidth;
   }
}

//=================================================================================================================================
///
/// SmartGrowMaxCharBufSize (private) - Intelligently grows the size of all the vertex buffers needed to render the font.
///   this also creates the corresponding index buffer to access this data
///
/// \param newSize - The new size of the buffers
///
/// \return void
//=================================================================================================================================
void Font::SmartGrowMaxCharBufSize( int32 newSize )
{
   if ( m_maxCharBufSize >= newSize )
   {
      return;
   }

   if ( m_maxCharBufSize == 0 )
   {
      m_maxCharBufSize = newSize;
   }
   else
   {
      while ( m_maxCharBufSize < newSize )
      {
         m_maxCharBufSize *= 2;
      }
   }

   if ( m_indBufferHandle == 0 )
   {
      glGenBuffers( 1, &m_indBufferHandle );
   }
   if ( m_posBufferHandle == 0 )
   {
      glGenBuffers( 1, &m_posBufferHandle );
   }
   if ( m_texBufferHandle == 0 )
   {
      glGenBuffers( 1, &m_texBufferHandle );
   }

   // 2 triangles per char (6 indices)
   int32 elementSize = m_maxCharBufSize * 6 * sizeof(GLshort);
   int32 elements = m_maxCharBufSize * 6;
   
   // x,y per vertex; 4 vertices per char
   int32 arraySize = m_maxCharBufSize * 2 * 4 * sizeof(GLfloat);

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_indBufferHandle );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, elementSize, NULL, GL_STATIC_DRAW );
   glBindBuffer( GL_ARRAY_BUFFER, m_posBufferHandle );
   glBufferData( GL_ARRAY_BUFFER, arraySize, NULL, GL_DYNAMIC_DRAW );
   glBindBuffer( GL_ARRAY_BUFFER, m_texBufferHandle );
   glBufferData( GL_ARRAY_BUFFER, arraySize, NULL, GL_DYNAMIC_DRAW );

   assert( elements <= 0xffff );

   // Fill in the element buffer
   GLushort *elementBuf = new GLushort[elements];
   for( int32 i = 0; i < elements; ++i )
   {
      GLushort remap[] = { 0, 1, 3, 3, 0, 2 };
      elementBuf[i] = i / 6 * 4 + remap[i%6];
   }
   glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, elements * sizeof(GLushort), elementBuf ); 
   delete [] elementBuf;

   if ( m_systemPosBuffer )
   {
      delete [] m_systemPosBuffer;
   }
   if ( m_systemTexBuffer )
   {
      delete [] m_systemTexBuffer;
   }
   m_systemPosBuffer = new float32[ arraySize/sizeof(float32) ];
   m_systemTexBuffer = new float32[ arraySize/sizeof(float32) ];
}

//=================================================================================================================================
///
/// UpdateVertexBuffers (private) - Updates all the vertex buffers with the data needed to render this font
///
/// \param str - The string we want to render
/// \param x - The x position we will be rendering the string at
/// \param y - The y position we will be rendering the string at
///
/// \return void
//=================================================================================================================================
void Font::UpdateVertexBuffers( const char8 *str, float32 x, float32 y )
{
   int32 strLength = int32(strlen( str ));
   SmartGrowMaxCharBufSize( strLength );
   float32 cursorOffset = 0;
   float32 xoff = x*2-1;
   float32 yoff = y*2-1;

   for ( int32 i = 0; i < strLength; ++i )
   {
      int32 charIndex = str[i];
      m_systemPosBuffer[i*8+0]  =  m_charData[charIndex].minX + cursorOffset + xoff;
      m_systemPosBuffer[i*8+1]  =  m_charData[charIndex].minY + yoff;
      m_systemPosBuffer[i*8+2]  =  m_charData[charIndex].minX + cursorOffset + xoff;
      m_systemPosBuffer[i*8+3]  =  m_charData[charIndex].maxY + yoff;
      m_systemPosBuffer[i*8+4]  =  m_charData[charIndex].maxX + cursorOffset + xoff;
      m_systemPosBuffer[i*8+5]  =  m_charData[charIndex].minY + yoff;
      m_systemPosBuffer[i*8+6]  =  m_charData[charIndex].maxX + cursorOffset + xoff;
      m_systemPosBuffer[i*8+7]  =  m_charData[charIndex].maxY + yoff;

      cursorOffset += m_charData[charIndex].xAdvance;
      
      m_systemTexBuffer[i*8+0]  = m_charData[charIndex].minS;
      m_systemTexBuffer[i*8+1]  = m_charData[charIndex].minT;
      m_systemTexBuffer[i*8+2]  = m_charData[charIndex].minS;
      m_systemTexBuffer[i*8+3]  = m_charData[charIndex].maxT;
      m_systemTexBuffer[i*8+4]  = m_charData[charIndex].maxS;
      m_systemTexBuffer[i*8+5]  = m_charData[charIndex].minT;
      m_systemTexBuffer[i*8+6]  = m_charData[charIndex].maxS;
      m_systemTexBuffer[i*8+7]  = m_charData[charIndex].maxT;
   }

   // x,y per vertex; 4 vertices per char
   int32 arraySize = strLength * 2 * 4 * sizeof(GLfloat);

   glBindBuffer( GL_ARRAY_BUFFER, m_posBufferHandle );
   glBufferSubData( GL_ARRAY_BUFFER, 0, arraySize, m_systemPosBuffer ); 
   glBindBuffer( GL_ARRAY_BUFFER, m_texBufferHandle );
   glBufferSubData( GL_ARRAY_BUFFER, 0, arraySize, m_systemTexBuffer ); 
}

