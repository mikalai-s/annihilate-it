//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation of the ImageDataDecompressor class.  This class decompresses image data.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/ImageDataDecompressor.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

// ATI_compress.h needs windef
#include <windows.h>

#include "ImageDataDecompressor.h"
#include "ATI_compress.h"
#include "texcompress.h"
#include <memory.h>
#include <assert.h>

//=================================================================================================================================
///
/// Decompresses data using the ATI_TC format.
///
/// \param img - Image object containing info and data about the image.
///
/// \return pass or fail boolean
//=================================================================================================================================
bool ImageDataDecompressor::DecompressAtiTcEtc( ImageObject *img )
{
   uint32 nError;
   uint32 flag, signature;

   switch ( img->GetCompressedType() )
   {
   case IMG_ATITC_RGB:
      {
         assert( img->GetNumChannels() == 3 );
         flag = TXCMPR_RGB;
         signature = ATITC_SIGNATURE;
      }
      break;
   case IMG_ATITC_RGBA:
      {
         assert( img->GetNumChannels() == 4 );
         flag = TXCMPR_RGBA;
         signature = ATITC_SIGNATURE;
      }
      break;
   case IMG_ETC1:
      {
         assert( img->GetNumChannels() == 3 );
         flag = TXCMPR_RGB;
         signature = ETC_SIGNATURE;
      }
      break;
   case IMG_ETC3:
      {
         assert( img->GetNumChannels() == 4 );
         flag = TXCMPR_RGBA;
         signature = ETC_SIGNATURE;
      }
      break;
   case IMG_ETC5:
      {
         assert( img->GetNumChannels() == 4 );
         flag = TXCMPR_RGBA | TXCMPR_ALPHA_INTERP;
         signature = ETC_SIGNATURE;
      }
      break;
   default:
      assert( 0 );
      return false;
   }

   uint32 uncompressedSize = img->GetBpp()/8 * img->GetWidth() * img->GetHeight();

   if ( img->GetNumChannels() == 3 )
   {
      
   }
   else
   {
      assert( img->GetNumChannels() == 4 );
      flag = TXCMPR_RGBA;
   }

   nError = TxCompressDecode( signature, img->GetWidth(), img->GetHeight(), flag, 
                              img->GetCompressedDataPtr(), 0, TXCMPR_RGB_RGBA_ORDER, img->GetDataPtr(), uncompressedSize);

   if ( nError != TXCMPR_ERROR_NO_ERROR )
   {
      return false;
   }

   return true;
}

//=================================================================================================================================
///
/// Decompresses data using the ATI_3DC format.
///
/// \param img - Image object containing info and data about the image.
///
/// \return pass or fail boolean
//=================================================================================================================================
bool ImageDataDecompressor::Decompress3dc( ImageObject *img )
{
   uint32 sizeNeeded = 0;
   ATI_TC_ERROR result;
   ATI_TC_Texture srcTexture;
   ATI_TC_Texture dstTexture;
   ATI_TC_FORMAT compressFormat;

   if ( img->GetCompressedType() == IMG_ATI2N )
   {
      compressFormat = ATI_TC_FORMAT_ATI2N;
   }
   else if ( img->GetCompressedType() == IMG_ATI1N )
   {
      compressFormat = ATI_TC_FORMAT_ATI1N;
   }
   else
   {
      assert( 0 );
      return false;
   }


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

   // Init src texture
   srcTexture.dwSize = sizeof( srcTexture );
   srcTexture.dwWidth = img->GetWidth();
   srcTexture.dwHeight = img->GetHeight();
   srcTexture.dwPitch = 0;  // Not needed for compressed textures
   srcTexture.format = compressFormat;
   srcTexture.dwDataSize = ATI_TC_CalculateBufferSize( &srcTexture );
   srcTexture.pData = img->GetCompressedDataPtr();

   // Init dst texture
   dstTexture.dwSize = sizeof( dstTexture );
   dstTexture.dwWidth = img->GetWidth();
   dstTexture.dwHeight = img->GetHeight();
   dstTexture.dwPitch = 0; // Needs to be 0 or this doesn't work (bug in lib)
   dstTexture.format = ATI_TC_FORMAT_ARGB_8888;
   dstTexture.dwDataSize = ATI_TC_CalculateBufferSize( &dstTexture );
   dstTexture.pData = img->GetDataPtr();

   result = ATI_TC_ConvertTexture( &srcTexture, &dstTexture, NULL, NULL, NULL, NULL );

   if( result != ATI_TC_OK )
   {
      assert( 0 );
      return false;
   }

   // Need to mung the data down to 1 or 2 channels
   if ( compressFormat == ATI_TC_FORMAT_ATI2N )
   {
      uint32 x, y;
      uint8 *data = img->GetDataPtr();
      uint32 height = img->GetHeight();
      uint32 width = img->GetWidth();

      for( y = 0; y < height; ++y )
      {
         for( x = 0; x < width; ++x )
         {
            data[y * width * 4 + x * 4 + 0] = data[y * width * 4 + x * 4 + 1]; // compressor puts x in channel 1 so we move it to 0 (R) 
            data[y * width * 4 + x * 4 + 1] = data[y * width * 4 + x * 4 + 2];  // compressor puts y in channel 2 so we move it to 1 (R)
            data[y * width * 4 + x * 4 + 2] = 0;
            data[y * width * 4 + x * 4 + 3] = 255;
         }
      }
   }
   else
   {
      assert( compressFormat == ATI_TC_FORMAT_ATI1N );
      uint32 x, y;
      uint8 *data = img->GetDataPtr();
      uint32 height = img->GetHeight();
      uint32 width = img->GetWidth();

      for( y = 0; y < height; ++y )
      {
         for( x = 0; x < width; ++x )
         {
            // compressor copys data into channels 0, 1, 2
            data[y * width * 4 + x * 4 + 1] = 0;
            data[y * width * 4 + x * 4 + 2] = 0;
            data[y * width * 4 + x * 4 + 3] = 255;
         }
      }
   }

   return true;
}
