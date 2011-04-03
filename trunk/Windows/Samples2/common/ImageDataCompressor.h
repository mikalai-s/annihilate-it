//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Definitions for the ImageDataCompressor class.  This class compresses image data.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/ImageDataCompressor.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#ifndef _IMAGEDATACOMPRESSOR_H_
#define _IMAGEDATACOMPRESSOR_H_

#include "ArgDefines.h"
#include "ImageObject.h"

class ImageDataCompressor
{
public:
   /// \brief Compresses data using the ATI_TC format or ETC.
   bool CompressAtiTcEtc( ImageObject *img, CompressedType compType );

   /// \brief Compresses data using the ATI_3DC format.
   bool Compress3dc( ImageObject *img, uint32 channels );
private:
};


#endif // _IMAGEDATACOMPRESSOR_H_

