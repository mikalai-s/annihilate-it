//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation of the ImageObject class.  This class holds and manages all image data information.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/ImageObject.cpp#2 $ 
// 
// Last check-in:  $DateTime: 2008/03/21 10:01:35 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "msImageObject.h"
#include <algorithm>
#include "assert.h"

using namespace std;

//=================================================================================================================================
///
/// Constructor
///
/// \param width - The image width
/// \param height - The image height
/// \param numChannels - Number of channels (ie RGB=3, RGBA=4)
/// \param bitsPerChannel - The bits per channel
///
/// \return void
//=================================================================================================================================
msImageObject::msImageObject( GLuint width, GLuint height, GLuint numChannels, GLuint bitsPerChannel )
{
   GLuint totalSize;
   assert ( width > 0 );
   assert ( height > 0 );
   assert ( ( numChannels > 0 ) && ( numChannels <= 4 ) );
   assert ( bitsPerChannel == 8 );

   this->width = width;
   this->height = height;
   this->numChannels = numChannels;
   this->bitsPerChannel = bitsPerChannel;
   
   totalSize = this->width * this->height * this->numChannels * this->bitsPerChannel / sizeof(unsigned char);

   this->data = new unsigned char[totalSize];
   assert( this->data );

   this->compressedType = IMG_NONE;
   this->compressedSize = 0;
   this->compressedData = NULL;
}

//=================================================================================================================================
///
/// Destructor 
///
/// \param void
///
/// \return void
//=================================================================================================================================
msImageObject::~msImageObject()
{
   delete [] this->data;
   delete [] this->compressedData;
}

//=================================================================================================================================
///
/// Access a single component of a single pixel
///
/// \param x - x position of the pixel
/// \param y - y position of the pixel
/// \param channel - channel fo the pixel (ie R, G, B, A)
///
/// \return a reference to the pixel
//=================================================================================================================================
unsigned char& msImageObject::Pixel( GLuint x, GLuint y, GLuint channel )
{
   GLuint pixelPitch = this->numChannels * this->bitsPerChannel / 8;
   GLuint linePitch = this->width * pixelPitch;
   
   x = min( x, this->width );
   y = min( y, this->height );
   channel = min( channel, this->numChannels );

   return this->data[y * linePitch + x * pixelPitch + channel]; 
}

//=================================================================================================================================
///
/// Sets up the type for compressed data
///
/// \param type - the type of compression we will be doing
///
/// \return pass or fail bool
//=================================================================================================================================
bool msImageObject::SetupCompressedData( CompressedType type )
{
   if ( this->compressedData )
   {
      delete [] this->compressedData;
   }

   switch ( type )
   {
   case IMG_NONE:
   case IMG_ATITC_RGB:
   case IMG_ATITC_RGBA:
   case IMG_ATI2N:
   case IMG_ATI1N:
   case IMG_ETC1:
   case IMG_ETC3:
   case IMG_ETC5:
      break;
   default:
      assert( 0 );
   }

   this->compressedData = NULL;
   this->compressedType = type;
   this->compressedSize = 0;
   return true;
}

//=================================================================================================================================
///
/// Sets up the size and data buffer for a compressed texture
///
/// \param size - The size of the compressed data
///
/// \return void
//=================================================================================================================================
void msImageObject::SetCompressedSize( GLuint size )
{
   if ( this->compressedData )
   {
      delete [] this->compressedData;
   }
   this->compressedData = new unsigned char[size];
   assert( this->compressedData );

   this->compressedSize = size;
}


//=================================================================================================================================
///
/// Changes the main data (not compressed data) to red
///
/// \param void
///
/// \return void
//=================================================================================================================================
void msImageObject::MakeImageRed()
{
   GLuint x, y, c;

   for ( y = 0; y < this->height; ++y )
   {
      for ( x = 0; x < this->height; ++x )
      {
         Pixel( x, y, 0 ) = 255;

         for ( c = 1; c < this->numChannels; ++c )
         {
            Pixel( x, y, c ) = 0;
         }
      }
   }
}

//=================================================================================================================================
///
/// Changes the main data from RGBA -> ARGB or RGB -> 1RGB
///
/// \param void
///
/// \return True if image has three or four channels, false otherwise
//=================================================================================================================================
bool msImageObject::MakeImageARGB()
{
   GLuint x, y;

   if ( this->numChannels != 3 && this->numChannels != 4 )
      return false;

   if ( this->numChannels == 4 )
   {
      for ( y = 0; y < this->height; ++y )
      {
         for ( x = 0; x < this->height; ++x )
         {
            unsigned char r = Pixel( x, y, 0 );
            unsigned char g = Pixel( x, y, 1 );
            unsigned char b = Pixel( x, y, 2 );
            unsigned char a = Pixel( x, y, 3 );
            
            Pixel( x, y, 0 ) = b;
            Pixel( x, y, 1 ) = g;
            Pixel( x, y, 2 ) = r;
            Pixel( x, y, 3 ) = a;

         }
      }
   }
   else
   {      
      GLuint nPixelSize = 4 * this->bitsPerChannel / 8;
      unsigned char *pNewData = new unsigned char[nPixelSize * this->width * this->height];

      for ( y = 0; y < this->height; ++y )
      {
         for ( x = 0; x < this->height; ++x )
         {

            GLuint nOffset = y * this->width * nPixelSize + x * nPixelSize;
            
            unsigned char r = Pixel( x, y, 0 );
            unsigned char g = Pixel( x, y, 1 );
            unsigned char b = Pixel( x, y, 2 );
            
            pNewData[nOffset + 0 ] = b;
            pNewData[nOffset + 1 ] = g;
            pNewData[nOffset + 2 ] = r;
            pNewData[nOffset + 3 ] = 255;            
         }
      }

      delete [] this->data;
      this->data = pNewData;
      this->numChannels = 4;
   }

   return true;
}

