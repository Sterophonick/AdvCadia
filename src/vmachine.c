//Emulation of the machine itself
#include <libheart.h>
#include "lang.h"
#include "emu.h"
#include "sound.h"
#include "vram.h"
#include "s2650.h"
#include "vmachine.h"
u8 spritecollision[SPRITE_MAX];
u16 spritex[SPRITE_MAX];
u16 spritey[SPRITE_MAX];
s8 sprskip[SPRITE_MAX];
u8 sprcolor[SPRITE_MAX];
extern u8 t;
int cpl;
int overcalc;
int slice;
s32 row,column;
u8 oldimagery;
u8 oldpitch;
extern PAD keypad;

void DMAFastCopy(void* source, void* dest, unsigned int count, unsigned int mode)
{   REG_DMA3SAD = (unsigned int) source;
        REG_DMA3DAD = (unsigned int) dest;
        REG_DMA3CNT = count | mode;
   }

void collidesprites(void)
{   /* This doesn't use screen[], thus it is safe at any point in the frame. */

    t = 0xFF;
    if (collision(0, 1))
    {   t &=  ~1; // AND with %11111110 (clear bit 0)
    }
    if (collision(0, 2))
    {   t &=  ~2; // AND with %11111101 (clear bit 1)
    }
    if (collision(0, 3))
    {   t &=  ~4; // AND with %11111011 (clear bit 2)
    }
    if (collision(1, 2))
    {   t &=  ~8; // AND with %11110111 (clear bit 3)
    }
    if (collision(1, 3))
    {   t &= ~16; // AND with %11101111 (clear bit 4)
    }
    if (collision(2, 3))
    {   t &= ~32; // AND with %11011111 (clear bit 5)
    }
    memory[A_SPRITECOLLIDE] = t;
}

u8 collision(u8 first, u8 second)
{
	if(sprskip[first] || sprskip[second])
    {
		return FALSE;
    }

    if (second == 3) // bullet
    {   
		if(spritex[first] <= spritex[second] - 5 || spritex[first] >= spritex[second] + 5 || spritey[first] <= spritey[second] - 10 || spritey[first] >= spritey[second] + 10)
        {
			return FALSE;
        } 
		else
        {  
			return TRUE;
		}   
	}
    else
    {
		if(spritex[first] <= spritex[second] - 8 || spritex[first] >= spritex[second] + 8 || spritey[first] <= spritey[second] - 16 || spritey[first] >= spritey[second] + 16)
        {   
			return FALSE;
        } 
		else
		{
			return TRUE;
		}
	}   
}

void uvi(void)
{
   psu &= ~(PSU_S);
    memory[A_CHARLINE] =
    memory[A_P1PADDLE] =       // "The potentiometer is set to $FF
    memory[A_P2PADDLE] = 0xFF; // on the trailing edge of VRST."
	slice = (cpl * 207) - overcalc;
	s2650_execute();
    for (row = 0; row < 13; row++)
    {   for (column = 0; column < 16; column++)
        {
			ArcadiaMap[((row) * 32) + 7 + column] = memory[(0x1800 + (row * 16) + column)];
		}
	}
    for (row = 0; row < 13; row++)
    {   for (column = 0; column < 16; column++)
        {
			ArcadiaMap[((row+13 ) * 32) + 7 + column] = memory[(0x1A00 + (row * 16) + column)];
		}
	}
	slice = (cpl *  33) - overcalc;
	s2650_execute(); // rasters 207..239
    psu |= PSU_S;
    slice = (cpl *  22) - overcalc;
    s2650_execute();
    spritex[0] =               memory[A_SPRITE0X] - 27;
    spritey[0] = (s16) 256 - memory[A_SPRITE0Y];
    spritex[1] =               memory[A_SPRITE1X] - 27;
    spritey[1] = (s16) 256 - memory[A_SPRITE1Y];
    spritex[2] =               memory[A_SPRITE2X] - 27;
    spritey[2] = (s16) 256 - memory[A_SPRITE2Y];
    spritex[3] =               memory[A_SPRITE3X] - 27;
    spritey[3] = (s16) 256 - memory[A_SPRITE3Y];
    for (register int i= 0; i< 4; i++)
    {   // check sprite position
        if (memory[A_SPRITE0X + (2 * i)] > 227)
        {   // "If the HC is set to >227, the object is effectively
            // removed from the video field."
            sprskip[i] = TRUE;
        } else {   
			sprskip[i] = FALSE;
		}
   }
    memory[A_BGCOLLIDE] = 0xFF;
    if (memory[0x19BF] != oldimagery)
    { 
		oldimagery = memory[0x19BF];
		tiles();
    }
	collidesprites();
	sprite();
	psu &= ~(PSU_S); // clear Sense bit. Necessary, eg. CIRCUS
    memory[A_CHARLINE] = 0xFF;
	DMAFastCopy(&ArcadiaMap, &se_mem[31][0], 1024, 0x80000000);
	if (memory[A_PITCH] != oldpitch)
    {   oldpitch = memory[A_PITCH];
        playsound();
    }
}

void EmuInput()
{
	hrt_GetPad(&keypad);
	t = 0;
    if (hrt_IsKeyPressed(START))
    {   t |= 1;
    }
    if (hrt_IsKeyPressed(SELECT))
    {   t |= 2;
    }
    memory[A_CONSOLE] = t;

if (memory[A_BGCOLOUR] & 0x40) // paddle interpolation bit
        {   if (hrt_IsKeyPressed(LEFT))
            {   memory[A_P1PADDLE] = 1;
            } elif (hrt_IsKeyPressed(RIGHT))
            {   memory[A_P1PADDLE] = 254;
            } else
            {   memory[A_P1PADDLE] = 112;
        }   }
        else
        {   if (hrt_IsKeyPressed(UP))
            {   memory[A_P1PADDLE] = 1;
            } elif (hrt_IsKeyPressed(DOWN))
            {   memory[A_P1PADDLE] = 254;
            } else
            {   memory[A_P1PADDLE] = 112;
    }   }   
	if(hrt_IsKeyPressed(L) && hrt_IsKeyPressed(R))
	{
		emuMenu();
	}
}

void init()
{
	memcpy(&memory[0], REG_POGOFILEPTR, 4096);
	//memcpy(&memory[0], rom, 4096);
}
