//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Very simple tga loader.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Tga.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

////////////////////////////////////////////////////////////////////////////////////
//
//  TGA.cpp
//
//      Loads a 24-bit TGA image 
//
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include "msTga.h"

///
//  Macros
//
#define INVERTED_BIT            (1 << 5)

///
//  Types
//
#pragma pack(push,x1)                            // Byte alignment (8-bit)
#pragma pack(1)


typedef struct
{
    unsigned char   IdSize,
                    MapType,
                    ImageType;
    unsigned short  PaletteStart,
                    PaletteSize;
    unsigned char   PaletteEntryDepth;
    unsigned short  X,
                    Y,
                    Width,
                    Height;
    unsigned char   ColorDepth,
                    Descriptor;
               
}   TGA_HEADER;

#pragma pack(pop,x1)

////////////////////////////////////////////////////////////////////////////////////
//
//  Private Functions
//

////////////////////////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//


///
//  TGALoad()
//
int TGALoad(const char *Filename, char **Buffer, int *Width, int *Height, int *Depth)
{
    FILE        *fp = fopen( Filename, "rb" ) ;
    
    TGA_HEADER   Header;
    

    if(fp == NULL)
    {
        return 0;
    }

    fread(&Header, sizeof(TGA_HEADER), 1, fp);
	
	*Width=  Header.Width;
	*Height= Header.Height;
	*Depth=  Header.ColorDepth;

	if(Header.ColorDepth== 24)
    {
        RGBTRIPLE1 *Buffer24;

        Buffer24= (RGBTRIPLE1*)malloc(sizeof(RGBTRIPLE1) * (*Width) * (*Height));

        if(Buffer24)
        {
            int i=0;
            int x,
                y;

            fread(Buffer24, sizeof(RGBTRIPLE1), (*Width) * (*Height), fp);

            *Buffer= (char*) malloc(3 * (*Width) * (*Height));
            
            for(y = 0; y < *Height; y++)
                for(x = 0; x < *Width; x++)
                {
                    int Index= y * (*Width) + x;

                    if(!(Header.Descriptor & INVERTED_BIT))
                        Index= ((*Height) - 1 - y) * (*Width) + x;

                    (*Buffer)[(i * 3)]=      Buffer24[Index].rgbtRed;
                    (*Buffer)[(i * 3) + 1]=  Buffer24[Index].rgbtGreen;
                    (*Buffer)[(i * 3) + 2]=  Buffer24[Index].rgbtBlue;
                    
                    i++;
                }

            fclose(fp);


            free(Buffer24);
            return(1);
        }		
    }
	else if(Header.ColorDepth == 32){
        RGBQUAD1 *Buffer32;

        Buffer32= (RGBQUAD1*)malloc(sizeof(RGBQUAD1) * (*Width) * (*Height));

        if(Buffer32)
        {
            int i=0;
            int x,
                y;

            fread(Buffer32, sizeof(RGBQUAD1), (*Width) * (*Height), fp);

            *Buffer= (char*) malloc(4 * (*Width) * (*Height));
            
            for(y = 0; y < *Height; y++)
                for(x = 0; x < *Width; x++)
                {
                    int Index= y * (*Width) + x;

                    if(!(Header.Descriptor & INVERTED_BIT))
                        Index= ((*Height) - 1 - y) * (*Width) + x;

                    (*Buffer)[(i * 4)]=      Buffer32[Index].rgbRed;
                    (*Buffer)[(i * 4) + 1]=  Buffer32[Index].rgbGreen;
                    (*Buffer)[(i * 4) + 2]=  Buffer32[Index].rgbBlue;
                    (*Buffer)[(i * 4) + 3]=  Buffer32[Index].rgbReserved;
                   
                    i++;
                }

            fclose(fp);


            free(Buffer32);
            return(1);
        }		
    }
    return(0);
}
