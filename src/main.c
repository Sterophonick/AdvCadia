#include <libheart.h>
#include "s2650.h"
#include "vram.h"
#include "sound.h"
#include "emu.h"
#include "lang.h"
#include "vmachine.h"
INCBIN(rom, "cattrax.bin"); //This is the test ROM, Cat Trax. Not included.

u32 autosleeptimer;
PAD keypad;
void emuMain();

int main() 
{
	hrt_Init();
	iar = 0;
    cpl = 53;
    psu = 0x62;
    psl = 0x02;
    ras[0] = 0x172;
    ras[1] = 0x986;
    r[0] = 0x10;
    r[1] = 0;
    r[2] = 0x4B;
    r[3] = 1;
	hrt_EnableRTC();
	hrt_BGSetMapBase(2,0x1F);
	hrt_SetBGX(3,24);
	hrt_DSPSetBGMode(2);
	hrt_BGSetSize(2, 1);
	hrt_DSPEnableBG(2);
	hrt_DSPDisableForceBlank();
	hrt_FXEnableOBJ(0);
	hrt_FXEnableBG(0,0);
	//hrt_BGSet256Color(0);
	hrt_DSPEnableOBJ();
	hrt_DSPEnableLinearOBJ();
	//Initialize();
	hrt_FXSetBlendMode(FX_MODE_DARKEN);
	hrt_FXEnableBG(2,0);
	autosleeptimer = 0;
	//updatevsync = 1;
	palette();
	chars();
	init();
    REG_SNDSTAT   = SSTAT_ON;
    REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR2_ON | SDMG_NOISE_ON, 7);
    REG_SNDDSCNT  = SDS_DMG100;
	REG_SND2CNT   = 0xF080;
    REG_SND2FREQ  = 0;
    REG_SND4CNT   = 0xF080;
    REG_SND4FREQ  = 0;
	hrt_SetOBJOffset(0,0);
	hrt_SetOBJOffset(1,1);
	hrt_SetOBJOffset(2,2);
	hrt_SetOBJOffset(3,3);
	hrt_EnableOBJAffine(0);
	hrt_EnableOBJAffine(1);
	hrt_EnableOBJAffine(2);
	hrt_EnableOBJAffine(3);
	hrt_AffineOBJ(0, 0, 256, 256);
	hrt_AffineOBJ(1, 0, 256, 256);
	hrt_AffineOBJ(2, 0, 256, 256);
	hrt_AffineOBJ(3, 0, 256, 256);
	for(u8 i=4;i<127;i++)
	{
		hrt_SetOBJX(i,255);
	}
	//sprites[127].attribute0 |= 0x200;
	oldimagery = 0xFF;
	scale();
	hrt_SetKeyStructPointer(keypad);
	emuMain();
	return 0;
}

void emuMain()
{
	CopyVRAM = 1;
	while(1)
	{
		//hrt_SleepF(Advcadia->vsync);
		EmuInput(); //Gets the input for the controllers
		uvi(); //Machine Emulation
		hrt_CopyOAM(); //Copies Sprite Data
	}
}