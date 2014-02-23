/*************************************
 * Includes
 ************************************/
#include <string.h>
#include <stdio.h>

#include "GBASys.h"
#include "Mem.h"
#include "Misc.h"
#include "BuildCnt.h"
#include "Media/SmallFont.h"
#include "Lua/lua.h"

/*************************************
 * Multiboot issues
 ************************************/

//should be sent by the makefile if to be
//compiled in multiboot
#ifdef COMPILE_MULTIBOOT
#define MULTIBOOT const int __gba_multiboot;
MULTIBOOT
#endif

#ifdef MULTIBOOT
#define STARTPOS   0x0203FFFC
#define DATAOFFSET 0x02000000
#else

/**
 * This variable is used to find where the masterfile ended and where
 * to find the data appended by GBALuaAppend.
 */
extern unsigned __ewram_overlay_lma; //i dont know if this is the proper symbol
#define STARTPOS   ((unsigned)&__ewram_overlay_lma)+1
#define DATAOFFSET 0x08000000

#endif

/*************************************
 * Functions
 ************************************/

/**
 * This functions is somewhat magic. Needs to be commented. Basicly
 * it searches and register data appended by GBALuaAppend into the
 * lua system. If an error is encountered an errormessage is displayed.
 */
void RegisterSymbols(lua_State *L){

  unsigned *currHdrPos=(unsigned*)STARTPOS;
  //this check could be alot smarter
  if((*currHdrPos)>0x40000){
    GBAAssert(*currHdrPos,"Couldn't find data! GBALua couldn't find any data appended by the GBALuaAppend program.");
  }
  
  currHdrPos=(unsigned*)(*currHdrPos+DATAOFFSET);

  unsigned short foundLuaCode=0;
  unsigned *firstDataPos=(unsigned*)STARTPOS;

  while(currHdrPos!=firstDataPos){
    unsigned lengthOfName=*currHdrPos;

    currHdrPos++;
    
    char name[100];
    strncpy(name,(char*)currHdrPos,lengthOfName);

    currHdrPos+=lengthOfName>>2;
    unsigned lengthOfData=*currHdrPos;
    currHdrPos++;

    if(strcmp(name,"LuaCode")==0){
      lua_dobuffer(L,(const char*)(unsigned*)(*currHdrPos+DATAOFFSET),lengthOfData,name);
      foundLuaCode=1;
    }
    lua_pushnumber(L,(unsigned)(*currHdrPos+DATAOFFSET));
    lua_setglobal(L,name);
      
    lua_pushnumber(L,lengthOfData);
    strcat(name,"_SIZE");
    lua_setglobal(L,name);

#ifndef MULTIBOOT
    if(firstDataPos==(unsigned*)STARTPOS){
      firstDataPos=(unsigned*)(*currHdrPos+DATAOFFSET);
    }
#endif
    
    currHdrPos++;
  }

  if(foundLuaCode==0){
    GBAAssert(1,"No lua code could be found in the appended data. The lua code must be namned 'LuaCode' (case sensitive)");
  }
}

/**
 * Shows a message when the lua program has exited. As most programs never leaves
 * an infinite loop they won't get to this function. The function waits for a keypress
 * and then the program is restarted. This function may be removed later, causing the
 * lua program to be restarted immediatly.
 */
void showExitMsg(void){
  unsigned short counter;
  unsigned short tileCounter;
  
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
  
  char exitMsg1[]="You have reached the";
  char exitMsg2[]=" end of the program.";
  char exitMsg3[]="   Press start to   ";
  char exitMsg4[]="restart the program!";
  
  char exitMsg5[]=" GBALua by superike ";
  char exitMsg6[]="dan@netbabyworld.com";
  
  unsigned pos=64+5;
  counter=0;
  while(exitMsg1[counter]!=0){
    MAP_1[pos]=exitMsg1[counter]-' ';
    counter++;
    pos++;
  }

  pos=96+5;
  counter=0;
  while(exitMsg2[counter]!=0){
    MAP_1[pos]=exitMsg2[counter]-' ';
    counter++;
    pos++;;
  }

  pos=128+5;
  counter=0;
  while(exitMsg3[counter]!=0){
    MAP_1[pos]=exitMsg3[counter]-' ';
    counter++;
    pos++;
  }
  
  pos=128+37;
  counter=0;
  while(exitMsg4[counter]!=0){
    MAP_1[pos]=exitMsg4[counter]-' ';
    counter++;
    pos++;
  }
  
  pos=128+68+129;
  counter=0;
  while(exitMsg5[counter]!=0){
    MAP_1[pos]=exitMsg5[counter]-' ';
    counter++;
    pos++;
  }
  
  pos=228+129;
  counter=0;
  while(exitMsg6[counter]!=0){
    MAP_1[pos]=exitMsg6[counter]-' ';
    counter++;
    pos++;
  }

  short done=0;
  while(done!=2){

    while(REG_VCOUNT!=160){}
    while(REG_VCOUNT!=161){}

    if(!(REG_P1&J_START)){
      done=1;
    }else{
      if(done==1){
        done=2;
      }
    }
  }
}

/**
 * Entry point of program (sort of anyway...). Starts up a lua environment,
 * and makes sure all functions and symbols are added to it. Then fires
 * the lua code up.
 */
void AgbMain(void){
  lua_State*    L=NULL;
  int           top;

  //go on forever.
  while(1){

    //reset the backgrounds (not needed?)
    REG_DISPCNT =0x0000;
    REG_BG0CNT  =0x0000;
    REG_BG1CNT  =0x0000;
    REG_BG2CNT  =0x0000;
    REG_BG3CNT  =0x0000;
    
    //start lua system
    L=lua_open(100);

    //register the build number
    lua_pushnumber(L,BUILD_COUNT);
    lua_setglobal(L,"GBALUA_BUILD_NR");

    //register some misc functions
    RegisterMisc(L);

    //register memory functions
    RegisterMem(L);
    
    //register that where appended with GBALuaAppend
    RegisterSymbols(L);
    
    //set garbage collection threshold, set very low or it will crash! (4000 made it crash)
    lua_setgcthreshold(L,40);
    
    //start the lua code at entrypoint "main"
    top=lua_gettop(L);
    lua_getglobal(L,"main");
    
    if(lua_type(L,1)==1){
      GBAAssert(1,"Couldn't find the function 'main'. This function must exist because it is the entry point of the program.");
    }

    lua_call(L,0,0);
    lua_settop(L,top);
    
    //close the lua system
    lua_close(L);

    showExitMsg();
  }
}


