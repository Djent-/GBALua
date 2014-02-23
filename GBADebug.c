/*************************************
 * Includes
 ************************************/
#include <stdio.h>
#include "GBASys.h"
#include "Media/SmallFont.h"

/*************************************
 * Functions
 ************************************/

int LUAAssert(lua_State *L){
  GBAAssert(lua_tonumber(L,1),(char *)lua_tostring(L,2));
  return 0;
}

void GBAAssert(int cond,char *msg){
  if(cond!=0){
    unsigned short counter;
    unsigned short tileCounter;

    unsigned short lineCounter=1;
    unsigned short rowCounter=0;
    
    //First init palette
    //Font palette goes in palette 0 (16 offset)
    for(counter=0;counter<SMALLFONT_PALETTE_COLORS;counter++){
      BG_PALETTE[counter]=SmallFontPaletteData[counter];
    }

    //Now copy the tiles  
    for(tileCounter=0;tileCounter<SMALLFONT_NR_OF_TILES;tileCounter++){
      for(counter=0;counter<SMALLFONT_TILESIZE;counter++){
        SCREEN_16K[(tileCounter*SMALLFONT_TILESIZE)+counter]=SmallFontTileData[tileCounter][counter];
      }
    }
  
    //Now clear the MAP_1 so that it doesn't contain alot of junk
    //the highest nibble is the palette
    for(counter=0;counter<1024;counter++){
      MAP_1[counter]=0x0000;
    }

    REG_DISPCNT=0x0240;

    //Enable and set up the bg
    //0000 0001 0000 0100
    REG_BG1CNT=0x0104;
  
    REG_BG1VOFS=0;
    REG_BG1HOFS=0;

    MAP_1[0]='A'-' ';
    MAP_1[1]='s'-' ';
    MAP_1[2]='s'-' ';
    MAP_1[3]='e'-' ';
    MAP_1[4]='r'-' ';
    MAP_1[5]='t'-' ';
    MAP_1[6]=':'-' ';
    
    char num[100];
    sprintf(num,"0x%08x",cond);
    for(counter=0;counter<10;counter++){
      MAP_1[counter+8]=num[counter]-' ';
    }

    counter=0;
    while(msg[counter]!=0){
      MAP_1[(lineCounter*32)+rowCounter]=msg[counter]-' ';
      
      rowCounter++;
      if(rowCounter==30){
        lineCounter++;
        rowCounter=0;
      }
      counter++;
    }

    while(1);
  }
}
