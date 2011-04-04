//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation of the ImageLoader class.  This class loads image files using the FreeImage lib.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/ImageLoader.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "ImageLoader.h"
#include "FreeImage.h"
#include <iostream>
#include <assert.h>

using namespace std;

//=================================================================================================================================
///
/// Constructor - Starts up the FreeImage lib.
///
/// \param void
///
/// \return void
//=================================================================================================================================
ImageLoader::ImageLoader()
{
   FreeImage_Initialise();
}

//=================================================================================================================================
///
/// Destructor - Shuts down the FreeImage lib.
///
/// \param void
///
/// \return void
//=================================================================================================================================
ImageLoader::~ImageLoader()
{
   FreeImage_DeInitialise();
}

//=================================================================================================================================
///
/// Helper funtion to load an image from a file.
///
/// \param filename - The filename string
/// \param flags - FreeImage flags
///
/// \return Pointer to a structure with all the FreeImage data in it
//=================================================================================================================================
static FIBITMAP* GenericLoader( const char8 *filename, int32 nFlags = 0 )
{
   FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

   // check the file signature and deduce its format
   // (the second argument is currently not used by FreeImage)
   fif = FreeImage_GetFileType( filename, 0 );

   if( fif == FIF_UNKNOWN )
   {
      // no signature ?
      // try to guess the file format from the file extension
      fif = FreeImage_GetFIFFromFilename( filename );
   }

   // check that the plugin has reading capabilities ...
   if( fif == FIF_UNKNOWN || FreeImage_FIFSupportsReading(fif) == false )
   {
      cout << "Cannot load " << filename << ": Unsupported file format" << endl;
      return NULL;
   }

   FIBITMAP *dib = FreeImage_Load( fif, filename, nFlags );
   // unless a bad file format, we are done !
   return dib;
}

//=================================================================================================================================
///
/// Helper funtion to load an image from a file.
///
/// \param pBitmap - Pointer to a structure with all the FreeImage data in it
///
/// \return The number channels
//=================================================================================================================================
uint32 GetNumChannels( FIBITMAP* pBitmap )
{
   assert( pBitmap != NULL );
   
   switch( FreeImage_GetImageType(pBitmap) )
   {
   case FIT_BITMAP:
      switch( FreeImage_GetBPP(pBitmap) )
      {
      case 1: return 1;
      case 16: return 2;
      case 24: return 3;
      case 4:      // palettized (I think)
      case 32:
         return 4;

      case 8:
         return 1;

      default:
         assert( !"Unknown FreeImage bitmap bit depth" );
         break;
      };
      break;

   case FIT_UINT16:
   case FIT_INT16:
   case FIT_UINT32:
   case FIT_INT32:
   case FIT_FLOAT:
   case FIT_DOUBLE:
      return 1;

   case FIT_COMPLEX:
      return 2;

   case FIT_RGB16:
   case FIT_RGBF:
      return 3;

   case FIT_RGBA16:
   case FIT_RGBAF:
      return 4;

   default:
      assert( !"Unknown FreeImage image type" );
      break;
   }

   return 0;
}

//=================================================================================================================================
///
/// Loads an image from a file.
///
/// \param filename - The image filename
///
/// \return Pointer to a structure with all the FreeImage data in it
//=================================================================================================================================
ImageObject* ImageLoader::LoadImageFile( const char8 *filename )
{
   FIBITMAP* pFIImg = GenericLoader( filename );
   if( pFIImg == NULL )
   {
      return NULL;
   }

   if( FreeImage_GetBPP( pFIImg ) < 8 )
   {
         cout << "<8 BPP is not supported" << endl;
         return NULL;
   }

   // Data seems to get flipped in freeImage so I'm going to flip it back
   FreeImage_FlipVertical( pFIImg );

   uint32 width = FreeImage_GetWidth( pFIImg );
   uint32 height = FreeImage_GetHeight( pFIImg );
   uint32 numChannels = GetNumChannels( pFIImg );   
   uint32 bitsPerChannel = FreeImage_GetBPP( pFIImg ) / numChannels;

   if( bitsPerChannel != 8 )
   {
      cout << "Image data with " << bitsPerChannel << " bits per channel not yet implemented";
      FreeImage_Unload( pFIImg );
      return NULL;
   }

   ImageObject *pImg = new ImageObject( width, height, numChannels, bitsPerChannel );
   assert( pImg != NULL );

   for( uint32 y = 0; y < pImg->GetHeight(); y++ )
   {
      for( uint32 x = 0; x < pImg->GetWidth(); x++ )
      {
         RGBQUAD pix;
         FreeImage_GetPixelColor( pFIImg, x, y, &pix );

         if ( pImg->GetNumChannels() == 1 )
         {
            BYTE val;
            FreeImage_GetPixelIndex( pFIImg, x, y, &val );
            pImg->Pixel(x, y, 0) = val;
         }
         else if ( pImg->GetNumChannels() == 2 )
         {
            assert( 0 );
         }
         else if ( pImg->GetNumChannels() == 3 )
         {
            pImg->Pixel(x, y, 0) = pix.rgbRed;
            pImg->Pixel(x, y, 1) = pix.rgbGreen;
            pImg->Pixel(x, y, 2) = pix.rgbBlue;
         }
         else if ( pImg->GetNumChannels() == 4 )
         {
            pImg->Pixel(x, y, 0) = pix.rgbRed;
            pImg->Pixel(x, y, 1) = pix.rgbGreen;
            pImg->Pixel(x, y, 2) = pix.rgbBlue;
            pImg->Pixel(x, y, 3) = pix.rgbReserved;
         }
         else
         {
            assert( 0 );
         }
      }
   }

   FreeImage_Unload( pFIImg );
   pFIImg = NULL;

   return pImg;
}

//=================================================================================================================================
///
/// Helper funtion to save an image from a file.
///
/// \param lpszPathName - The filename string
/// \param flags - FreeImage flags
/// \param dib - a structure with all the FreeImage data in it
///
/// \return pass or fail bool
//=================================================================================================================================
static bool GenericWriter(FIBITMAP* dib, const char* lpszPathName, int flag) 
{
   FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
   BOOL bSuccess = FALSE;

   if(dib)
   {
      // try to guess the file format from the file extension
      fif = FreeImage_GetFIFFromFilename(lpszPathName);
      if(fif != FIF_UNKNOWN )
      {
         // check that the plugin has sufficient writing and export capabilities ...
         WORD bpp = FreeImage_GetBPP(dib);
         if(FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp))
         {
            // ok, we can save the file
            bSuccess = FreeImage_Save(fif, dib, lpszPathName, flag);
            // unless an abnormal bug, we are done !
         }
      }
   }
   return (bSuccess == TRUE) ? true : false;
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
bool ImageLoader::SaveImage( const char8 *filename, ImageObject *imgObj )
{
   FIBITMAP *pFIImg;
   bool ret;

   pFIImg = FreeImage_AllocateT( FIT_BITMAP, imgObj->GetWidth(), imgObj->GetHeight(), imgObj->GetBpp() );

   for( uint32 y = 0; y < imgObj->GetHeight(); y++ )
   {
      for( uint32 x = 0; x < imgObj->GetWidth(); x++ )
      {
         if ( imgObj->GetNumChannels() == 1 )
         {
            assert( 0 );
         }
         else if ( imgObj->GetNumChannels() == 2 )
         {
            assert( 0 );
         }
         else if ( imgObj->GetNumChannels() == 3 )
         {
            RGBQUAD pix;

            pix.rgbRed   = imgObj->Pixel(x, y, 0);
            pix.rgbGreen = imgObj->Pixel(x, y, 1);
            pix.rgbBlue  = imgObj->Pixel(x, y, 2);
            FreeImage_SetPixelColor( pFIImg, x, y, &pix );
         }
         else if ( imgObj->GetNumChannels() == 4 )
         {
            RGBQUAD pix;

            pix.rgbRed       = imgObj->Pixel(x, y, 0);
            pix.rgbGreen     = imgObj->Pixel(x, y, 1);
            pix.rgbBlue      = imgObj->Pixel(x, y, 2);
            pix.rgbReserved  = imgObj->Pixel(x, y, 3);
            FreeImage_SetPixelColor( pFIImg, x, y, &pix );
         }
         else
         {
            assert( 0 );
         }
      }
   }

   ret = GenericWriter( pFIImg, filename, 0 );

   FreeImage_Unload( pFIImg );
   pFIImg = NULL;

   return ret;
}

