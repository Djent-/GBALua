#ifndef GBASYS_H
#define GBASYS_H

#include "GBAIrq.h"
#include "GBAJoy.h"
#include "GBAReg.h"
#include "GBADebug.h"

#define GAMEOVER_LEVEL_CLEAR  1
#define GAMEOVER_GIVE_UP      2
#define GAMEOVER_LOST         4
#define GAMEOVER_TIME_UP      8
#define GAMEOVER_ERROR        16

//Palette
#define BG_PALETTE        ((unsigned short*)0x05000000)
#define OBJ_PALETTE       ((unsigned short*)0x05000200)

//VRAM
#define MAP_0             ((unsigned short*)0x06000000)
#define MAP_1             ((unsigned short*)0x06000800)
#define MAP_2             ((unsigned short*)0x06001000)
#define MAP_3             ((unsigned short*)0x06001800)
#define MAP_4             ((unsigned short*)0x06002000)
#define SCREEN_0K         ((unsigned char*) 0x06000000)
#define SCREEN_16K        ((unsigned short*)0x06004000)
#define SCREEN_32K        ((unsigned short*)0x06008000)
#define SCREEN_48K        ((unsigned short*)0x0600C000)

#define M4_FRAME0             ((unsigned short*) 0x06000000)
#define M4_FRAME1             ((unsigned short*) 0x0600A000)
#define SPRITE_DATA           ((unsigned short*)0x06010000)
#define SPRITE_DATA_BITMAPPED ((unsigned short*)0x06014000)

//OAM
typedef struct _Sprite {
  unsigned short Attrib0;
  unsigned short Attrib1;
  unsigned short Attrib2;
  unsigned short RotateScale;
} Sprite;

#define OAM                   ((Sprite*)0x07000000)


//SRAM / EEPROM
#define FLASH_1M              ((unsigned char*)0x09fe0000)          // 1M Flash Memory
#define FLASH_1M_END          ((unsigned char*)0x0a000000)

#endif //GBASYS_H
