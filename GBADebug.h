#ifndef GBADEBUG_H
#define GBADEBUG_H

#include "Lua/lua.h"

int LUAAssert(lua_State *L);
void GBAAssert(int cond,char *msg);

#endif
