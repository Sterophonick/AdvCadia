extern u8 collision(u8 first, u8 second);
extern void collidesprites(void);
extern u8 sprcolor[4];
extern u8 spritecollision[4];
extern u16 spritex[4];
extern u16 spritey[4];
extern s8 sprskip[4];
extern u8 rom[4096];
extern void init();
extern void uvi();
extern int cpl;
extern u8 oldimagery;
extern void DMAFastCopy(void* source, void* dest, unsigned int count, unsigned int mode);
#define SPRITE_MAX 4

#define A_BOXWIDTH       160 // must be a multiple of 4!
#define A_BOXHEIGHT      262

#define BOXWIDTH         A_BOXWIDTH
#define BOXHEIGHT        A_BOXHEIGHT

#define A_OFFSET_UPPERSCREEN 0x1800
#define A_OFFSET_LOWERSCREEN 0x1A00
#define A_OFFSET_SPRITES     0x1980
#define A_OFFSET_UDCS        0x19A0

#define A_SPRITE0Y       6384 // $18F0 R/W
#define A_SPRITE0X       6385 // $18F1 R/W
#define A_SPRITE1Y       6386 // $18F2 R/W
#define A_SPRITE1X       6387 // $18F3 R/W
#define A_SPRITE2Y       6388 // $18F4 R/W
#define A_SPRITE2X       6389 // $18F5 R/W
#define A_SPRITE3Y       6390 // $18F6 R/W
#define A_SPRITE3X       6391 // $18F7 R/W
#define A_VSCROLL        6396 // $18FC -/W?
#define A_PITCH          6397 // $18FD -/W?
#define A_VOLUME         6398 // $18FE -/W?
#define A_CHARLINE       6399 // $18FF (dma)
#define A_P1LEFTKEYS     6400 // $1900
#define A_P1MIDDLEKEYS   6401 // $1901
#define A_P1RIGHTKEYS    6402 // $1902
#define A_P1PALLADIUM    6403 // $1903
#define A_P2LEFTKEYS     6404 // $1904
#define A_P2MIDDLEKEYS   6405 // $1905
#define A_P2RIGHTKEYS    6406 // $1906
#define A_P2PALLADIUM    6407 // $1907
#define A_CONSOLE        6408 // $1908
#define A_RESOLUTION     6648 // $19F8
#define A_BGCOLOUR       6649 // $19F9
#define A_SPRITES23CTRL  6650 // $19FA
#define A_SPRITES01CTRL  6651 // $19FB
#define A_BGCOLLIDE      6652 // $19FC
#define A_SPRITECOLLIDE  6653 // $19FD
#define A_P2PADDLE       6654 // $19FE
#define A_P1PADDLE       6655 // $19FF

#define P1_LEFT 0x01
#define P1_RIGHT 0x2
#define P1_DOWN 0x04
#define P1_UP 0x08

extern void EmuInput();

typedef unsigned short      SB_ENTRY;
typedef SB_ENTRY            SCREENBLOCK[32*32];
#define se_mem              ((SCREENBLOCK*)0x06000000)

void emuMain();