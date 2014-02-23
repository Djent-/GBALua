#ifndef DECODEPCX_H
#define DECODEPCX_H

#include "main.h"

bool HandlePCXHeader(FILE *fileHandle,_PCXHeader *pcxHeader);
bool DecodePCXData(FILE *fileHandle,_PCXHeader *pcxHeader,unsigned char *screenData);

#endif
