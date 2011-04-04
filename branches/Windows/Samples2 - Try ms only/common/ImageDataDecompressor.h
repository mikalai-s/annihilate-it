//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Definitions for the ImageDataDecompressor class.  This class decompresses image data.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/ImageDataDecompressor.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#ifndef _IMAGEDATADECOMPRESSOR_H_
#define _IMAGEDATADECOMPRESSOR_H_

#include "ImageObject.h"

class ImageDataDecompressor
{
public:
   /// \brief Decompresses data using the ATI_TC format or ETC.
   bool DecompressAtiTcEtc( ImageObject *img );

   /// \brief Decompresses data using the ATI_3DC format.
   bool Decompress3dc( ImageObject *img );

private:
};

#endif // _IMAGEDATADECOMPRESSOR_H_

