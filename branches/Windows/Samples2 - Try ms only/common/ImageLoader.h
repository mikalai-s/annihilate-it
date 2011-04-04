//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Definetions for the ImageLoader class.  This class loads image files using the FreeImage lib.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/ImageLoader.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#ifndef _IMAGELOADER_H_
#define _IMAGELOADER_H_

#include "ArgDefines.h"
#include "ImageObject.h"
#include "FreeImage.h"

class ImageLoader
{
public:
   /// \brief Constructor
   ImageLoader();

   /// \brief Deconstructor
   ~ImageLoader();

   /// \brief Loads an image from a file.
   ImageObject* LoadImageFile( const char8 *fileName );

   /// \brief Saves an image to a file.
   bool SaveImage( const char8 *fileName, ImageObject *imgObj );
};

#endif // _IMAGELOADER_H_

