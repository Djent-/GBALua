#ifndef PARAMS_H
#define PARAMS_H

#include "main.h"

struct _GLOBALS{
  bool m_showFileHeader;
  bool m_silet;
  bool m_generateDepend;
  bool m_invertPalette;
  bool m_constTiles;
  bool m_totalSilence;
  
  bool m_writeTiles;
  bool m_writePalette;
  bool m_writeMap;
  
  bool m_writeHeaderFile;
  bool m_writeCFile;
  bool m_writePCXFile;

  bool m_exportBinary;
  
  bool m_insertZeroTile;
  
  int  m_retValue;

  unsigned m_tileXSize;
  unsigned m_tileYSize;

  bool m_optimizeTiles;
  bool m_exportBitmap;

  bool m_colors16;
};

void InitGlobals(void);
void PrintHelp(void);
std::list<std::string> HandleParams(int argc,char *argv[]);

extern _GLOBALS globals;
      
#endif