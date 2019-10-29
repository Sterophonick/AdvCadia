#include <libheart.h>
#include "s2650.h"
#include "vram.h"
#include "sound.h"
#include "emu.h"
#include "lang.h"
#include "vmachine.h"
emu* Advcadia = (emu*)0xE000000;
u8 emuMenuArrowPos;
u8 emuMenuKeyLockUp;
u8 emuMenuKeyLockDown;
u8 emuMenuKeyLockLeft;
u8 emuMenuKeyLockRight;
u8 emuMenuKeyLockA;
extern PAD keypad;
u8 CopyVRAM;

//Other Options
void emuMenuHelp();
void emuMenuControls();
void emuMenuDisplay();
void emuMenuOther();
void emuMenuReset();
void emuMenuBack();
void emuMenuExit();
void emuMenuSleep();

//Helpers
void emuMenuHelperReInit();
void emuMenuHelperRTC();
void emuMenuHelperFadeOut();
void emuMenuHelperFadeIn();

void emuMenu()
{
	emuMenuHelperFadeOut();
	if(CopyVRAM) hrt_DMA_Copy(3, VRAM, &VRAM[0x4000], 0x4000, 0x80000000);
	CopyVRAM = 0;
	hrt_InitTiledText(0);
	hrt_DSPSetBGMode(0);
	hrt_DSPDisableOBJ();
	REG_SOUND2CNT_H = 0;
	hrt_ClearTiledText();
	hrt_DSPDisableBG(2);
	hrt_DSPEnableBG(0);
	emuMenuHelperLoadMenu(gl_main_menu_options, 6);
	emuMenuHelperFadeIn();
	hrt_GetPad(&keypad);
	while(!hrt_IsKeyPressed(B))
	{
		emuMenuHelperMain(6, emuMenuMainOptions);
	}
	hrt_DMA_Copy(3, &VRAM[0x4000], VRAM, 0x4000, 0x80000000);
	hrt_BGSetMapBase(2,0x1F);
	hrt_SetBGX(3,24);
	hrt_DSPSetBGMode(2);
	hrt_BGSetSize(2, 1);
	hrt_DSPEnableBG(2);
	hrt_DSPDisableForceBlank();
	hrt_DSPEnableOBJ();
	hrt_DSPEnableLinearOBJ();
	palette();
	chars();
	playsound();
	emuMain();
}

void emuMenuHelp()
{
	emuMenuHelperFadeOut();
	hrt_ClearTiledText();
	emuMenuHelperLoadMenu(gl_help_text, 10);
	emuMenuHelperFadeIn();
	hrt_GetPad(&keypad);
	while(!hrt_IsKeyPressed(B))
	{
		hrt_VblankIntrWait();
		hrt_GetPad(&keypad);
	}
	emuMenu();
}

void emuMenuControls()
{
	emuMenuHelperFadeOut();
	hrt_ClearTiledText();
	emuMenuHelperLoadMenu(gl_controller_options, 5);
	hrt_PrintOnTilemap(8, 7, gl_keypad_options[Advcadia->a_key]);
	hrt_PrintOnTilemap(8, 8, gl_keypad_options[Advcadia->b_key]);
	hrt_PrintOnTilemap(13, 9, gl_autofire_options[Advcadia->a_auto]);
	hrt_PrintOnTilemap(13, 10, gl_autofire_options[Advcadia->b_auto]);
	emuMenuHelperFadeIn();
	hrt_GetPad(&keypad);
	while(!hrt_IsKeyPressed(B))
	{
		emuMenuHelperMain(4,emuMenuControllerOptions);
	}
	emuMenu();
}

void emuMenuDisplay()
{
	emuMenuHelperFadeOut();
	hrt_ClearTiledText();
	emuMenuHelperLoadMenu(gl_display_options, 2);
	hrt_PrintOnTilemap(12, 7, gl_brightness_options[Advcadia->brightness]);
	emuMenuHelperFadeIn();
	hrt_GetPad(&keypad);
	while(!hrt_IsKeyPressed(B))
	{
		emuMenuHelperMain(1,emuMenuDisplayOptions);
	}
	emuMenu();
}

void emuMenuOther()
{

}

void emuMenuReset()
{
	hrt_SoftReset();
}

void ATTR_NORETURN emuMenuBack()
{
	asm("bx lr");
}

void emuMenuSleep()
{
	hrt_Suspend();
}

void emuMenuExit()
{
	hrt_EZ4Exit();
}

void emuMenuControllerAKey()
{
	Advcadia->a_key++;
	if(Advcadia->a_key > 11)
		Advcadia->a_key = 0;
	hrt_PrintOnTilemap(8, 7, gl_keypad_options[Advcadia->a_key]);
}

void emuMenuControllerBKey()
{
	Advcadia->b_key++;
	if(Advcadia->b_key > 11)
		Advcadia->b_key = 0;
	hrt_PrintOnTilemap(8, 8, gl_keypad_options[Advcadia->b_key]);
}

void emuMenuControllerAAuto()
{
	Advcadia->a_auto++;
	if(Advcadia->a_auto > 2)
		Advcadia->a_auto = 0;
	hrt_PrintOnTilemap(13, 9, gl_autofire_options[Advcadia->a_auto]);
}

void emuMenuControllerBAuto()
{
	Advcadia->b_auto++;
	if(Advcadia->b_auto > 2)
		Advcadia->b_auto = 0;
	hrt_PrintOnTilemap(13, 10, gl_autofire_options[Advcadia->b_auto]);
}

void emuMenuDisplayBrightness()
{
	Advcadia->brightness++;
	if(Advcadia->brightness > 4)
		Advcadia->brightness = 0;
	hrt_PrintOnTilemap(12, 7, gl_brightness_options[Advcadia->brightness]);
}

void emuMenuHelperFadeOut()
{
	for(register int i=0;i<16;i++)
	{
		hrt_VblankIntrWait();
		hrt_SetFXLevel(i);
	}
}

void emuMenuHelperFadeIn()
{
	for(register int i=0;i<16;i++)
	{
		hrt_VblankIntrWait();
		hrt_SetFXLevel(16-i);
	}
}

void emuMenuHelperRTC() {
	char str[9];
	hrt_PrintRTCTimeIntoString(str);
	hrt_PrintOnTilemap(22, 0, str);
}