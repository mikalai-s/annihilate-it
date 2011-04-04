//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation of the ImageDataCompressor class.  This class compresses image data.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/ImageDataCompressor.cpp#2 $ 
// 
// Last check-in:  $DateTime: 2008/03/21 10:01:35 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

// ATI_compress.h needs windef
#include <windows.h>

#include "ImageDataCompressor.h"
#include "ImageObject.h"
#include "assert.h"
#include "ATI_compress.h"
#include <memory>

#define NULL 0
#define MIN_COMPRESSED_SIZE 16

//=================================================================================================================================
///
/// Compresses data using the ATI_TC format or ETC.
///
/// \param img - Image object containing info and data about the image.
///
/// \return pass or fail boolean
//=================================================================================================================================
bool ImageDataCompressor::CompressAtiTcEtc( ImageObject *img, CompressedType compType )
{
   ATI_TC_ERROR result;
   ATI_TC_Texture srcTexture;
   ATI_TC_Texture dstTexture;
   ATI_TC_FORMAT compressFormat;
   ATI_TC_FORMAT srcFormat;
   
   img->SetupCompressedData( compType );
      
   assert( img->GetNumChannels() == 3 || img->GetNumChannels() == 4 );
   switch ( compType )
   {
   case IMG_ATITC_RGB:
      {         
         compressFormat = ATI_TC_FORMAT_ATC_RGB;
      }
      break;
   case IMG_ATITC_RGBA:
      {
         assert( img->GetNumChannels() == 4 );         
         compressFormat = ATI_TC_FORMAT_ATC_RGBA_Explicit;
      }
      break;
   case IMG_ETC1:
      {         
         compressFormat = ATI_TC_FORMAT_ETC_RGB;    
      }
      break;
   case IMG_ETC3:      
   case IMG_ETC5:      
   default:
      assert( 0 );
   }

   // Convert image to ARGB
   img->MakeImageARGB();
   srcFormat = ATI_TC_FORMAT_ARGB_8888;
         
   
   // Init src texture
   srcTexture.dwSize = sizeof( srcTexture );
   srcTexture.dwWidth = img->GetWidth();
   srcTexture.dwHeight = img->GetHeight();
   srcTexture.dwPitch = img->GetWidth() * img->GetBpp() / 8;
   srcTexture.format = srcFormat;
   srcTexture.dwDataSize = ATI_TC_CalculateBufferSize( &srcTexture );
   srcTexture.pData = img->GetDataPtr();

   // Init dst texture
   dstTexture.dwSize = sizeof( dstTexture );
   dstTexture.dwWidth = img->GetWidth();
   dstTexture.dwHeight = img->GetHeight();
   dstTexture.dwPitch = 0; // Not needed for compressed textures
   dstTexture.format = compressFormat;
   dstTexture.dwDataSize = ATI_TC_CalculateBufferSize( &dstTexture );
   img->SetCompressedSize( dstTexture.dwDataSize );
   dstTexture.pData = img->GetCompressedDataPtr();

   result = ATI_TC_ConvertTexture( &srcTexture, &dstTexture, NULL, NULL, NULL, NULL );

   if( result != ATI_TC_OK )
   {
      assert( 0 );
      return false;
   }
   return true;
}

//=================================================================================================================================
///
/// Compresses data using the ATI_3DC format.
///
/// \param img - Image object containing info and data about the image.
/// \param channels - 1=ATI1N, 2=ATI2N
///
/// \return pass or fail boolean
//=================================================================================================================================
bool ImageDataCompressor::Compress3dc( ImageObject *img, uint32 channels )
{
   uint32 sizeNeeded = 0;
   ATI_TC_ERROR result;
   ATI_TC_Texture srcTexture;
   ATI_TC_Texture dstTexture;
   ATI_TC_FORMAT compressFormat;

   if ( img->GetNumChannels() != 4 )
   {
      // Only support 4 channel textures for this test
      assert( 0 );
      return false;
   }
   
   if ( img->GetBitsPerChannel() != 8 )
   {
      assert( 0 );
      return false;
   }

   if ( channels == 1 )
   {
      compressFormat = ATI_TC_FORMAT_ATI1N;
      img->SetupCompressedData( IMG_ATI1N );
   }
   else if ( channels == 2 )
   {
      compressFormat = ATI_TC_FORMAT_ATI2N;
      img->SetupCompressedData( IMG_ATI2N );
   }
   else
   {
      assert( 0 );
      return false;
   }

   // Init src texture
   srcTexture.dwSize = sizeof( srcTexture );
   srcTexture.dwWidth = img->GetWidth();
   srcTexture.dwHeight = img->GetHeight();
   srcTexture.dwPitch = img->GetWidth() * img->GetBpp() / 8;
   srcTexture.format = ATI_TC_FORMAT_ARGB_8888;
   srcTexture.dwDataSize = ATI_TC_CalculateBufferSize( &srcTexture );
   srcTexture.pData = img->GetDataPtr();

   // Init dst texture
   dstTexture.dwSize = sizeof( dstTexture );
   dstTexture.dwWidth = img->GetWidth();
   dstTexture.dwHeight = img->GetHeight();
   dstTexture.dwPitch = 0; // Not needed for compressed textures
   dstTexture.format = compressFormat;
   dstTexture.dwDataSize = ATI_TC_CalculateBufferSize( &dstTexture );
   img->SetCompressedSize( dstTexture.dwDataSize );
   dstTexture.pData = img->GetCompressedDataPtr();

   result = ATI_TC_ConvertTexture( &srcTexture, &dstTexture, NULL, NULL, NULL, NULL );

   if( result != ATI_TC_OK )
   {
      assert( 0 );
      return false;
   }
   return true;
}

