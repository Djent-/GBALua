#ifndef FILEWRITE_H
#define FILEWRITE_H

#include "main.h"

void WriteOutFiles(std::string fileName,
                   std::list<unsigned char*> tileList,
                   unsigned short *tileMap,
                   std::list<_RGBColor*> palette,
                   _PCXHeader *pcxHeader);

void WriteBinaries(std::string upperCaseDataName,
                   std::list<unsigned char*> tileList,
                   unsigned short *tileMap,
                   std::list<_RGBColor*> palette,
                   _PCXHeader *pcxHeader);
  

void WriteHeaderFile(std::string fileName,
                     std::string dataName,
                     std::string upperCaseDataName,
                     std::list<unsigned char*> tileList,
                     _PCXHeader *pcxHeader);

void WriteCFile(std::string fileName,
    std::string dataName,
    std::string upperCaseDataName,
    std::list<unsigned char*> tileList,
    unsigned short *tileMap,
    std::list<_RGBColor*> palette,
    _PCXHeader *pcxHeader);
    
void WritePCXFile(std::string fileName,
                  std::list<unsigned char*> tileList,
                  std::list<_RGBColor*> palette,
                  _PCXHeader *pcxHeader);

#endif