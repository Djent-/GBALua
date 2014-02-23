/*************************************
 * Includes
 ************************************/
#include <stdio.h>
#include <math.h>

#include "Mem.h"

/*************************************
 * Functions
 ************************************/

int MemCopy8(lua_State *L){
  
  char  *dest=   (char*)lua_tonumber(L,1);
  char  *source= (char*)lua_tonumber(L,2);
  int   len=lua_tonumber(L,3);
  
  unsigned counter;
  for(counter=0;counter<len;counter++){
    dest[counter]=source[counter];
  }
  return 0;
}

int MemCopy16(lua_State *L){

  short *dest=   (short*)lua_tonumber(L,1);
  short *source= (short*)lua_tonumber(L,2);
  int   len=lua_tonumber(L,3);

  unsigned counter;
  for(counter=0;counter<len;counter++){
    dest[counter]=source[counter];
  }
  return 0;
}

int MemCopy32(lua_State *L){
  
  int *dest=   (int*)lua_tonumber(L,1);
  int *source= (int*)lua_tonumber(L,2);
  int len=lua_tonumber(L,3);
  
  unsigned counter;
  for(counter=0;counter<len;counter++){
    dest[counter]=source[counter];
  }
  return 0;
}

int MemCopy8WithAdd(lua_State *L){
  
  char  *dest=  (char*)lua_tonumber(L,1);
  char  *source=(char*)lua_tonumber(L,2);
  char  add=lua_tonumber(L,3);
  int   len=lua_tonumber(L,4);

  unsigned counter;
  for(counter=0;counter<len;counter++){
    dest[counter]=source[counter]+add;
  }
  return 0;
}

int MemCopy16WithAdd(lua_State *L){
  
  unsigned short *dest=  (unsigned short*)lua_tonumber(L,1);
  unsigned short *source=(unsigned short*)lua_tonumber(L,2);
  unsigned short add=lua_tonumber(L,3);
  int  len=lua_tonumber(L,4);

  char num[20];
  sprintf(num,"%x\n",add);
  
  unsigned counter;
  for(counter=0;counter<len;counter++){
    dest[counter]=source[counter]+add;
  }
  return 0;
}

int MemCopy32WithAdd(lua_State *L){
  
  int   *dest=  (int*)lua_tonumber(L,1);
  int   *source=(int*)lua_tonumber(L,2);
  int   add=lua_tonumber(L,3);
  int  len=lua_tonumber(L,4);
  
  unsigned counter;
  for(counter=0;counter<len;counter++){
    dest[counter]=source[counter]+add;
  }
  return 0;
}

int MemFill8(lua_State *L){

  char *dest=(char*)lua_tonumber(L,1);
  char fillValue=lua_tonumber(L,2);
  int len=lua_tonumber(L,3);

  unsigned counter;
  for(counter=0;counter<len;counter++){
    dest[counter]=fillValue;
  }
  return 0;
}

int MemFill16(lua_State *L){
  
  short *dest=(short*)lua_tonumber(L,1);
  short fillValue=lua_tonumber(L,2);
  int len=lua_tonumber(L,3);
  
  unsigned counter;
  for(counter=0;counter<len;counter++){
    dest[counter]=fillValue;
  }
  return 0;
}

int MemFill32(lua_State *L){
  
  int *dest=(int*)lua_tonumber(L,1);
  int fillValue=lua_tonumber(L,2);
  int len=lua_tonumber(L,3);
  
  unsigned counter;
  for(counter=0;counter<len;counter++){
    dest[counter]=fillValue;
  }
  return 0;
}

int MemRead8(lua_State *L){
  unsigned char *dest=(unsigned char*)lua_tonumber(L,1);
  lua_pushnumber(L,*dest);
  return 1;
}

int MemRead16(lua_State *L){
  unsigned short *dest=(unsigned short*)lua_tonumber(L,1);
  lua_pushnumber(L,*dest);
  return 1;
}

int MemRead32(lua_State *L){
  unsigned*dest=(unsigned*)lua_tonumber(L,1);
  lua_pushnumber(L,*dest);
  return 1;
}

void RegisterMem(lua_State* L){
  lua_register(L,"MemCopy8",MemCopy8);
  lua_register(L,"MemCopy16",MemCopy16);
  lua_register(L,"MemCopy32",MemCopy32);
  lua_register(L,"MemCopy8WithAdd",MemCopy8WithAdd);
  lua_register(L,"MemCopy16WithAdd",MemCopy16WithAdd);
  lua_register(L,"MemCopy32WithAdd",MemCopy32WithAdd);
  lua_register(L,"MemFill8",MemFill8);
  lua_register(L,"MemFill16",MemFill16);
  lua_register(L,"MemFill32",MemFill32);
  lua_register(L,"MemRead8",MemRead8);
  lua_register(L,"MemRead16",MemRead16);
  lua_register(L,"MemRead32",MemRead32);
}

