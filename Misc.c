/*************************************
 * Includes
 ************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Lua/lua.h"
#include "GBADebug.h"

/*************************************
 * Functions
 ************************************/

 /*
  supporting function that takes a string
  that spells a hex value and converts it
  to a number.
*/
int Hex(lua_State *L){
  char *endptr;
  int value=strtoul((char*)lua_tostring(L,1),&endptr,0);
  lua_pushnumber(L,value);

  return 1;
}

int band(lua_State *L){
  int a=lua_tonumber(L,1);
  int b=lua_tonumber(L,2);
  lua_pushnumber(L,(a&b));
  
  return 1;
}

int bor(lua_State *L){
  int a=lua_tonumber(L,1);
  int b=lua_tonumber(L,2);
  lua_pushnumber(L,(a|b));
  
  return 1;
}

int bxor(lua_State *L){
  int a=lua_tonumber(L,1);
  int b=lua_tonumber(L,2);
  lua_pushnumber(L,(a^b));
  
  return 1;
}

int StrCopy(lua_State *L){
  
  short *dest=  (short*)lua_tonumber(L,1);
  char  *source=(char*)lua_tostring(L,2);

  unsigned counter=0;
  while(source[counter]!=0){
    dest[counter]=source[counter]-' ';
    counter++;
  }
  return 0;
}

int StrLen(lua_State *L){
  lua_pushnumber(L,strlen(lua_tostring(L,1)));
  return 1;
}

int NumToStr(lua_State *L){
  char num[20];
  sprintf(num,"%u\n",lua_tonumber(L,1));
  lua_pushstring(L,num);
  return 1;
}

void RegisterMisc(lua_State* L){
  //register some supporting functions, these are all subject
  //to be renamed, redesigned or removed from the system...
  lua_register(L,"assert",LUAAssert);
  lua_register(L,"StrCopy",StrCopy);    //create a str.c for string functions?
  lua_register(L,"StrLen",StrLen);      //create a str.c for string functions?
  lua_register(L,"NumToStr",NumToStr);  //create a str.c for string functions?
  lua_register(L,"Hex",Hex);            //is this the best way?

  lua_register(L,"band",band);
  lua_register(L,"bor",bor);
  lua_register(L,"bxor",bxor);
}

