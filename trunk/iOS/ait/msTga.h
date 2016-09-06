//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Very simple tga loader.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Tga.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#ifndef _TGA_H_
#define _TGA_H_

typedef struct tagRGBTRIPLE1 {
    char    rgbtBlue;
    char    rgbtGreen;
    char    rgbtRed;
} RGBTRIPLE1;

typedef struct tagRGBQUAD1 {
    char rgbBlue;
    char rgbGreen;
    char rgbRed;
    char rgbReserved;
} RGBQUAD1;


int TGALoad(const char *Filename, char **Buffer, int *Width, int *Height, int *Depth);

#endif // _TGA_H_