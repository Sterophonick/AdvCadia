//Tile to VRAM conversion
#include <libheart.h>
#include "lang.h"
#include "emu.h"
#include "sound.h"
#include "vram.h"
#include "s2650.h"
#include "vmachine.h"
int x;
int y;
int xx,yy;
u8 t;

u8 ArcadiaMap[1024];
u16* ArcadiaTiles = (u16*)0x60144a0;
u8 SpriteImagery[4][32];
s32 imagedata;

extern void setArcadiaTiles(int i, int y);

const u8 characters[56][8] =
{   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //  0 (space)
    { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 }, //  1 (/)
    { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 }, //  2 (\)
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, //  3 (solid block)
    { 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //  4
    { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }, //  5
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF }, //  6 (_)
    { 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0 }, //  7
    { 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }, //  8
    { 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0 }, //  9
    { 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF }, // 10
    { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF }, // 11
    { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF }, // 12
    { 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF }, // 13
    { 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80 }, // 14
    { 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 }, // 15
    { 0x00, 0x1c, 0x22, 0x26, 0x2a, 0x32, 0x22, 0x1c }, // 16 0
    { 0x00, 0x08, 0x18, 0x08, 0x08, 0x08, 0x08, 0x1c }, // 17 1
    { 0x00, 0x1c, 0x22, 0x02, 0x0c, 0x10, 0x20, 0x3e }, // 18 2
    { 0x00, 0x3e, 0x02, 0x04, 0x0c, 0x02, 0x22, 0x1c }, // 19 3
    { 0x00, 0x04, 0x0c, 0x14, 0x24, 0x3e, 0x04, 0x04 }, // 20 4
    { 0x00, 0x3e, 0x20, 0x3c, 0x02, 0x02, 0x22, 0x1c }, // 21 5
    { 0x00, 0x0c, 0x10, 0x20, 0x3c, 0x22, 0x22, 0x1c }, // 22 6
    { 0x00, 0x7c, 0x02, 0x04, 0x08, 0x10, 0x10, 0x10 }, // 23 7
    { 0x00, 0x1c, 0x22, 0x22, 0x1c, 0x22, 0x22, 0x1c }, // 24 8
    { 0x00, 0x1c, 0x22, 0x22, 0x3e, 0x02, 0x04, 0x18 }, // 25 9
    { 0x00, 0x08, 0x14, 0x22, 0x22, 0x3e, 0x22, 0x22 }, // 26 A
    { 0x00, 0x3c, 0x22, 0x22, 0x3c, 0x22, 0x22, 0x3c }, // 27 B
    { 0x00, 0x1c, 0x22, 0x20, 0x20, 0x20, 0x22, 0x1c }, // 28 C
    { 0x00, 0x3c, 0x22, 0x22, 0x22, 0x22, 0x22, 0x3c }, // 29 D
    { 0x00, 0x3e, 0x20, 0x20, 0x3c, 0x20, 0x20, 0x3e }, // 30 E
    { 0x00, 0x3e, 0x20, 0x20, 0x3c, 0x20, 0x20, 0x20 }, // 31 F
    { 0x00, 0x1e, 0x20, 0x20, 0x20, 0x26, 0x22, 0x1e }, // 32 G
    { 0x00, 0x22, 0x22, 0x22, 0x3e, 0x22, 0x22, 0x22 }, // 33 H
    { 0x00, 0x1c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c }, // 34 I
    { 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x22, 0x1c }, // 35 J
    { 0x00, 0x22, 0x24, 0x28, 0x30, 0x28, 0x24, 0x22 }, // 36 K
    { 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3e }, // 37 L
    { 0x00, 0x22, 0x36, 0x2a, 0x2a, 0x22, 0x22, 0x22 }, // 38 M
    { 0x00, 0x22, 0x22, 0x32, 0x2a, 0x26, 0x22, 0x22 }, // 39 N
    { 0x00, 0x1c, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1c }, // 40 O
    { 0x00, 0x3c, 0x22, 0x22, 0x3c, 0x20, 0x20, 0x20 }, // 41 P
    { 0x00, 0x1c, 0x22, 0x22, 0x22, 0x2a, 0x24, 0x1a }, // 42 Q
    { 0x00, 0x3c, 0x22, 0x22, 0x3c, 0x28, 0x24, 0x22 }, // 43 R
    { 0x00, 0x1c, 0x22, 0x20, 0x1c, 0x02, 0x22, 0x1c }, // 44 S
    { 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 }, // 45 T
    { 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1c }, // 46 U
    { 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x14, 0x08 }, // 47 V
    { 0x00, 0x22, 0x22, 0x22, 0x2a, 0x2a, 0x36, 0x22 }, // 48 W
    { 0x00, 0x22, 0x22, 0x14, 0x08, 0x14, 0x22, 0x22 }, // 49 X
    { 0x00, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08 }, // 50 Y
    { 0x00, 0x3e, 0x02, 0x04, 0x08, 0x10, 0x20, 0x3e }, // 51 Z
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08 }, // 52 .
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x10 }, // 53 ,
    { 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00 }, // 54 +
    { 0x00, 0x08, 0x1E, 0x28, 0x1C, 0x0A, 0x3C, 0x08 }  // 55 $
};

void sprite()
{
    sprcolor[0] = 7-((memory[A_SPRITES01CTRL] & 0x38) >> 3)-1; // %00111000 -> %00000111
    sprcolor[1] = 7-( memory[A_SPRITES01CTRL] & 0x07      )-1; // %00000111 -> %00000111
    sprcolor[2] = 7-((memory[A_SPRITES23CTRL] & 0x38) >> 3)-1; // %00111000 -> %00000111
    sprcolor[3] = 7-( memory[A_SPRITES23CTRL] & 0x07      )-1; // %00000111 -> %00000111
	for(register int i=0; i<4;i++)
	{
		x = spritex[i];
        y = spritey[i];
        if (memory[A_SPRITE0Y + (i << 1)] == 0)
        {   
			hrt_SetOBJX(i, 240);
            hrt_SetOBJY(i, 160);
            continue;
        }
		else
        {  
			hrt_SetOBJX(i, x + 40);
            hrt_SetOBJY(i, ((y-17 ))*.77);
			SpriteImagery[i][(yy * 4) + (xx / 2)] &= 0x0F;
        }
        for (yy = 0; yy < 8; yy++) // for each y-line of the sprite
        {   
			y = spritey[i] + yy;
            imagedata = memory[0x1980 + (i * 8) + yy];
            SpriteImagery[i][(yy * 4)    ] =
            SpriteImagery[i][(yy * 4) + 1] =
            SpriteImagery[i][(yy * 4) + 2] =
            SpriteImagery[i][(yy * 4) + 3] = 0x0;

            for (xx = 0; xx < 8; xx++)
            {   
				if (imagedata & (0x80 >> xx)) // if this x-bit is set
                {  
					x = spritex[i] + xx;
                    if (xx & 1)
                    {   
						SpriteImagery[i][(yy * 4) + (xx / 2)] |= (sprcolor[i] + 1) << 4;
                    } 
					else
                    {   
						SpriteImagery[i][(yy * 4) + (xx / 2)] |= (sprcolor[i] + 1);
                    }
				}
			}
		}
	}
	DMAFastCopy(SpriteImagery, OAMData, 4*32, 0x80000000);
}


 int fgc1, fgc2, fgc3, fgc4;
 
 void chars()
 {
 
     int                 i,
                y; // all PERSISTent for speed
				
				    for (i = 0; i < 64 * 16; i++)
    {   ArcadiaTiles[i             ] =
        ArcadiaTiles[i + ( 64 * 16)] =
        ArcadiaTiles[i + (128 * 16)] =
        ArcadiaTiles[i + (192 * 16)] = 0x0000;
    }
     fgc1 = 7-((memory[A_BGCOLOUR] >> 3) &  1); // 0 or 1
    fgc2 = 7-((0x40) >> 5)  // %11000000 -> %00000110 (0, 2, 4 or 6)
         + ((memory[A_BGCOLOUR] >> 3) &  1); // 0 or 1
    fgc3 = 7-((0x80) >> 5)  // %11000000 -> %00000110 (0, 2, 4 or 6)
         + ((memory[A_BGCOLOUR] >> 3) &  1); // 0 or 1
    fgc4 = 7-((0xC0) >> 5)  // %11000000 -> %00000110 (0, 2, 4 or 6)
         + ((memory[A_BGCOLOUR] >> 3) &  1); // 0 or 1

   for (i = 0; i < 56; i++)
    {   for (y = 0; y < 8; y++)
        {   t = characters[i][y];
			setArcadiaTiles(i, y);
  }   }
 }

void tiles()
{
	BUP temp;
	temp.SrcNum = 8192;
	temp.SrcBitNum = 4;
	temp.DestBitNum = 8;
    int                 i,
                y; // all PERSISTent for speed
    /*for (i = 0; i < 64 * 16; i++)
    {   ArcadiaTiles[i             ] =
        ArcadiaTiles[i + ( 64 * 16)] =
        ArcadiaTiles[i + (128 * 16)] =
        ArcadiaTiles[i + (192 * 16)] = 0x0000;
    }
	*/

    for (i = 56; i < 64; i++)
    {   for (y = 0; y < 8; y++)
        {   t = memory[A_OFFSET_SPRITES + (8 * (i - 56)) + y];
			setArcadiaTiles(i, y);
}   }
	hrt_BitUnPack(&ArcadiaTiles[0], &tile_mem[0][0], &temp);
	//DMAFastCopy(&ArcadiaTiles[0], &tile_mem[0][0], 4096, 0x80000000);
}

void setArcadiaTiles(int i, int y)
{
            if (t & 128)
            {  ArcadiaTiles[( i        * 16) + (y * 2)]     |= fgc1;
                ArcadiaTiles[((i +  64) * 16) + (y * 2)]     |= fgc2;
                ArcadiaTiles[((i + 128) * 16) + (y * 2)]     |= fgc3;
                ArcadiaTiles[((i + 192) * 16) + (y * 2)]     |= fgc4;
            }
            if (t & 64)
            {  ArcadiaTiles[( i        * 16) + (y * 2)]     |= fgc1 << 4;
                ArcadiaTiles[((i +  64) * 16) + (y * 2)]     |= fgc2 << 4;
                ArcadiaTiles[((i + 128) * 16) + (y * 2)]     |= fgc3 << 4;
                ArcadiaTiles[((i + 192) * 16) + (y * 2)]     |= fgc4 << 4;
            }
            if (t & 32)
            {  ArcadiaTiles[( i        * 16) + (y * 2)]     |= fgc1 << 8;
                ArcadiaTiles[((i +  64) * 16) + (y * 2)]     |= fgc2 << 8;
                ArcadiaTiles[((i + 128) * 16) + (y * 2)]     |= fgc3 << 8;
                ArcadiaTiles[((i + 192) * 16) + (y * 2)]     |= fgc4 << 8;
            }
            if (t & 16)
            { 	ArcadiaTiles[( i        * 16) + (y * 2)]     |= fgc1 << 12;
                ArcadiaTiles[((i +  64) * 16) + (y * 2)]     |= fgc2 << 12;
                ArcadiaTiles[((i + 128) * 16) + (y * 2)]     |= fgc3 << 12;
                ArcadiaTiles[((i + 192) * 16) + (y * 2)]     |= fgc4 << 12;
            }
            if (t & 8)
            {  ArcadiaTiles[( i        * 16) + (y * 2) + 1] |= fgc1;
                ArcadiaTiles[((i +  64) * 16) + (y * 2) + 1] |= fgc2;
                ArcadiaTiles[((i + 128) * 16) + (y * 2) + 1] |= fgc3;
                ArcadiaTiles[((i + 192) * 16) + (y * 2) + 1] |= fgc4;
            }
            if (t & 4)
            {  ArcadiaTiles[( i        * 16) + (y * 2) + 1] |= fgc1 << 4;
                ArcadiaTiles[((i +  64) * 16) + (y * 2) + 1] |= fgc2 << 4;
                ArcadiaTiles[((i + 128) * 16) + (y * 2) + 1] |= fgc3 << 4;
                ArcadiaTiles[((i + 192) * 16) + (y * 2) + 1] |= fgc4 << 4;
            }
            if (t & 2)
            {  ArcadiaTiles[( i        * 16) + (y * 2) + 1] |= fgc1 << 8;
                ArcadiaTiles[((i +  64) * 16) + (y * 2) + 1] |= fgc2 << 8;
                ArcadiaTiles[((i + 128) * 16) + (y * 2) + 1] |= fgc3 << 8;
                ArcadiaTiles[((i + 192) * 16) + (y * 2) + 1] |= fgc4 << 8;
            }
            if (t & 1)
            {  ArcadiaTiles[( i        * 16) + (y * 2) + 1] |= fgc1 << 12;
                ArcadiaTiles[((i +  64) * 16) + (y * 2) + 1] |= fgc2 << 12;
                ArcadiaTiles[((i + 128) * 16) + (y * 2) + 1] |= fgc3 << 12;
                ArcadiaTiles[((i + 192) * 16) + (y * 2) + 1] |= fgc4 << 12;
		} 
}

#define SPRITE_Y_SCALE 325

void scale()
{
	BGAffineSource tempa;
	BGAffineDest tempb;
			tempa.x = 80*256;
			tempa.y = 80*256;
			tempa.tX = 80;
			tempa.tY = 61;
			tempa.sX = 256;
			tempa.sY = 331;
			tempa.theta = 0;
			hrt_BgAffineSet(&tempa, &tempb, 1);
			REG_BG2PA = tempb.pa;
			REG_BG2PB = tempb.pb;
			REG_BG2PC = tempb.pc;
			REG_BG2PD = tempb.pd;
			REG_BG2X = tempb.x;
			REG_BG2Y = tempb.y;
	hrt_AffineOBJ(0, 0, 256, SPRITE_Y_SCALE);
	hrt_AffineOBJ(1, 0, 256, SPRITE_Y_SCALE);
	hrt_AffineOBJ(2, 0, 256, SPRITE_Y_SCALE);
	hrt_AffineOBJ(3, 0, 256, SPRITE_Y_SCALE);
}

u16 bgpalettes[8] = {
	0x0000,0x7C00,0x001F,0x7C1F,0x03E0,0x7FE0,0x03FF,0xFFFF,
};

void palette()
{
	//BG Palette
	hrt_SetBGPalEntry(0, 0x0000);
	hrt_SetBGPalEntry(1, 0x7C00);
	hrt_SetBGPalEntry(2, 0x001F);
	hrt_SetBGPalEntry(3, 0x7C1F);
	hrt_SetBGPalEntry(4, 0x03E0);
	hrt_SetBGPalEntry(5, 0x7FE0);
	hrt_SetBGPalEntry(6, 0x03FF);
	hrt_SetBGPalEntry(7, 0xFFFF);
	//OBJ Palette
	hrt_SetOBJPalEntry(0, 0x0000);
	hrt_SetOBJPalEntry(1, 0x7C00);
	hrt_SetOBJPalEntry(2, 0x001F);
	hrt_SetOBJPalEntry(3, 0x7C1F);
	hrt_SetOBJPalEntry(4, 0x03E0);
	hrt_SetOBJPalEntry(5, 0x7FE0);
	hrt_SetOBJPalEntry(6, 0x03FF);
	hrt_SetOBJPalEntry(7, 0xFFFF);
}