#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <list>

struct _PCXHeader{
  unsigned char   m_manufacturer;
  unsigned char   m_version;
  unsigned char   m_encoding;
  unsigned char   m_bitsPerPixel;
  signed short    m_imageXMin;
  signed short    m_imageYMin;
  signed short    m_imageXMax;
  signed short    m_imageYMax;
  signed short    m_HDpi;
  signed short    m_VDpi;
  unsigned char   m_colorMap[48];
  unsigned char   m_reserved;
  unsigned char   m_nPlanes;
  signed short    m_bytesPerLine;
  signed short    m_paletteInfo;
  signed short    m_hScreenSize;
  signed short    m_vScreenSize;
  unsigned char   m_filler[54];
};

struct _RGBColor{
  unsigned char R;
  unsigned char G;
  unsigned char B;
};

#endif