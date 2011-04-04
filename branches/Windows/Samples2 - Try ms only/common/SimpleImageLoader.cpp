//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation of the SimpleImageLoader class.  This class loads only handles tga files and does not use FreeImage.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/SimpleImageLoader.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "SimpleImageLoader.h"
#include <iostream>
#include <assert.h>
#include "Tga.h"

using namespace std;

//=================================================================================================================================
///
/// Constructor - Starts up the FreeImage lib.
///
/// \param void
///
/// \return void
//=================================================================================================================================
SimpleImageLoader::SimpleImageLoader()
{

}

//=================================================================================================================================
///
/// Destructor - Shuts down the FreeImage lib.
///
/// \param void
///
/// \return void
//=================================================================================================================================
SimpleImageLoader::~SimpleImageLoader()
{

}

//=================================================================================================================================
///
/// Loads an image from a file.
///
/// \param filename - The image filename
///
/// \return Pointer to a structure with all the FreeImage data in it
//=================================================================================================================================
ImageObject* SimpleImageLoader::LoadImageFile( const char8 *filename )
{
   int ret;
   char *buf;
   int32 width, height, bpp, numChannels, bitsPerChannel;

   ret = TGALoad( filename, &buf, &width, &height, &bpp);

   if ( ! ret )
   {
      return NULL;
   }

   if ( bpp == 32 )
   {
      numChannels = 4;
      bitsPerChannel = 8;
   }
   else if ( bpp == 24 )
   {
      numChannels = 3;
      bitsPerChannel = 8;
   }
   else
   {
      assert(0);
      free(buf);
      return NULL;
   }

   ImageObject *pImg = new ImageObject( width, height, numChannels, bitsPerChannel );
   assert( pImg != NULL );

   for( uint32 y = 0; y < pImg->GetHeight(); y++ )
   {
      for( uint32 x = 0; x < pImg->GetWidth(); x++ )
      {
         if ( pImg->GetNumChannels() == 3 )
         {
            pImg->Pixel(x, y, 0) = buf[ width*y*3 + x*3 + 0 ];
            pImg->Pixel(x, y, 1) = buf[ width*y*3 + x*3 + 1 ];
            pImg->Pixel(x, y, 2) = buf[ width*y*3 + x*3 + 2 ];
         }
         else if ( pImg->GetNumChannels() == 4 )
         {
            pImg->Pixel(x, y, 0) = buf[ width*y*4 + x*4 + 0 ];
            pImg->Pixel(x, y, 1) = buf[ width*y*4 + x*4 + 1 ];
            pImg->Pixel(x, y, 2) = buf[ width*y*4 + x*4 + 2 ];
            pImg->Pixel(x, y, 3) = buf[ width*y*4 + x*4 + 3 ];
         }
         else
         {
            assert( 0 );
         }
      }
   }

   free(buf);

   return pImg;
}

//=================================================================================================================================
///
/// Saves an image to a file.
///
/// \param filename - The image filename
/// \param imgObj - class that holds image data
///
/// \return pass or fail bool
//=================================================================================================================================
bool SimpleImageLoader::SaveImage( const char8 *filename, ImageObject *imgObj )
{
   assert(0);
   return false;
}

