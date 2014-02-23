#include "FileWrite.h"
#include "Params.h"

void WriteOutFiles(std::string fileName,
                   std::list<unsigned char*> tileList,
                   unsigned short *tileMap,
                   std::list<_RGBColor*> palette,
                   _PCXHeader *pcxHeader){
  
  if(!globals.m_silet){
    printf("Writing file...(0%%)");
  }
  
  //Find data name
  std::string dataName=fileName;
  size_t pos=dataName.find_last_of(".",dataName.size());
  if(pos!=-1){
    dataName.resize(pos);
  }
  pos=dataName.find_last_of("\\",dataName.size());
  if(pos!=-1){
    dataName=dataName.substr(pos+1,dataName.size());
  }
  pos=dataName.find_last_of("/",dataName.size());
  if(pos!=-1){
    dataName=dataName.substr(pos,dataName.size());
  }
  
  char *tmpDataName=new char[dataName.size()+1];
  strcpy(tmpDataName,dataName.c_str());
  strupr(tmpDataName);
  std::string upperCaseDataName=tmpDataName;
  delete [] tmpDataName;
  
  std::string hFileName=dataName;
  hFileName+=".h";
  std::string cFileName=dataName;
  cFileName+=".c";
  std::string pcxFileName="Tiles_";
  pcxFileName+=dataName;
  pcxFileName+=".pcx";
  
  if(globals.m_exportBinary){
    WriteBinaries(upperCaseDataName,tileList,tileMap,palette,pcxHeader);
  }else{
    if(globals.m_writeHeaderFile){
      WriteHeaderFile(hFileName,dataName,upperCaseDataName,tileList,pcxHeader);
    }
    if(globals.m_writeCFile){
      WriteCFile(cFileName,dataName,upperCaseDataName,tileList,tileMap,palette,pcxHeader);
    }
  }
  
  
  if(globals.m_writePCXFile){
    WritePCXFile(pcxFileName,tileList,palette,pcxHeader);
  }
}

void WriteBinaries( std::string upperCaseDataName,
                    std::list<unsigned char*> tileList,
                    unsigned short *tileMap,
                    std::list<_RGBColor*> palette,
                    _PCXHeader *pcxHeader){

  if(globals.m_writeMap){
    std::string mapName=upperCaseDataName;
    mapName+="_MAP";
    std::string mapFileName=mapName;
    mapFileName+=".bin";
    
    FILE *mapFile=fopen(mapFileName.c_str(),"wb");

    unsigned screenXSize=pcxHeader->m_imageXMax-pcxHeader->m_imageXMin+1;
    unsigned screenYSize=pcxHeader->m_imageYMax-pcxHeader->m_imageYMin+1;
    
    unsigned mapSize=(screenYSize/globals.m_tileYSize)*(screenXSize/globals.m_tileXSize);
    fwrite(tileMap,mapSize,2,mapFile);
    fclose(mapFile);
  }
  if(globals.m_writePalette){
    std::string paletteName=upperCaseDataName;
    paletteName+="_PALETTE";
    std::string paletteFileName=paletteName;
    paletteFileName+=".bin";
    
    FILE *paletteFile=fopen(paletteFileName.c_str(),"wb");
    
    unsigned nrOfColors=0;
    if(globals.m_colors16){
      nrOfColors=16;
    }else{
      nrOfColors=256;
    }

    std::list<_RGBColor*>::iterator it;
    if(globals.m_invertPalette){
      it=palette.end();
      it--;
    }else{
      it=palette.begin();
    }
    
    unsigned short counter;
    for(counter=0;counter<nrOfColors;counter++){
      _RGBColor *rgbColor=(*it);
      
      unsigned short color=(rgbColor->R&0xf8)>>3;
      color+=(rgbColor->G&0xf8)<<2;
      color+=(rgbColor->B&0xf8)<<7;
      fwrite(&color,2,1,paletteFile);

      if(globals.m_invertPalette){
        it--;
      }else{
        it++;
      }
    }
    fclose(paletteFile);
  }

  unsigned char progress=0;
  if(globals.m_writeTiles){
    std::string tilesName=upperCaseDataName;
    tilesName+="_TILES";
    std::string tilesFileName=tilesName;
    tilesFileName+=".bin";
    
    FILE *tilesFile=fopen(tilesFileName.c_str(),"wb");

    std::list<unsigned char*>::iterator t=tileList.begin();
    unsigned tileCounter=0;
    while(t!=tileList.end()){
      unsigned long counterY;
      for(counterY=0;counterY<globals.m_tileYSize;counterY++){
        
        if(globals.m_colors16){
          //16 colors palette
          
          unsigned long counterX;
          for(counterX=0;counterX<globals.m_tileXSize/4;counterX++){
            unsigned short word=0;
            unsigned char counter;
            for(counter=0;counter<4;counter++){
              unsigned char pixel=(*t)[(counterY*(globals.m_tileXSize))+(counterX*4)+counter];
              if(globals.m_invertPalette){
                if(globals.m_insertZeroTile){
                  if(t!=tileList.begin()){
                    pixel^=0xff;
                  }
                }else{
                  pixel^=0xff;
                }
              }
              
              pixel&=0x0f;
              word+=pixel<<(counter*4);
            }
            
            fwrite(&word,2,1,tilesFile);
          }
          
        }else{
          unsigned long counterX;
          for(counterX=0;counterX<globals.m_tileXSize/2;counterX++){
            //256 colors palette
            
            unsigned short word=0;
            word=(*t)[(counterY*(globals.m_tileXSize))+(counterX*2)];
            word+=(*t)[(counterY*(globals.m_tileXSize))+(counterX*2)+1]<<8;
            
            if(globals.m_invertPalette){
              if(globals.m_insertZeroTile){
                if(t!=tileList.begin()){
                  word^=0xffff;
                }
              }else{
                word^=0xffff;
              }
            }
            
            fwrite(&word,2,1,tilesFile);
          }
        }
        
      }
      
      tileCounter++;
      
      if(!globals.m_silet){
        unsigned char currProgress=(unsigned char)(((float)tileCounter/(float)tileList.size())*100.0);
        if(progress!=currProgress){
          progress=currProgress;
          printf("\rWriting file...(%u%%)",progress);
        }
      }
      
      t++;
      
    }
      
    
    fclose(tilesFile);
  }
  
  if(!globals.m_silet){
    printf("\rWriting file...(100%%)\n");
  }
}
    



void WriteHeaderFile(std::string fileName,
                     std::string dataName,
                     std::string upperCaseDataName,
                     std::list<unsigned char*> tileList,
                     _PCXHeader *pcxHeader){

  unsigned screenXSize=pcxHeader->m_imageXMax-pcxHeader->m_imageXMin+1;
  unsigned screenYSize=pcxHeader->m_imageYMax-pcxHeader->m_imageYMin+1;
  
  FILE *hFile=fopen(fileName.c_str(),"w");
  
  fprintf(hFile,"#ifndef %s_H\n",upperCaseDataName.c_str());
  fprintf(hFile,"#define %s_H\n",upperCaseDataName.c_str());
  
  fprintf(hFile,"/* Tile size:        %u*%u */\n",globals.m_tileXSize,globals.m_tileYSize);
  fprintf(hFile,"/* Number of tiles:  %u */\n",tileList.size());
  if(globals.m_colors16){
    fprintf(hFile,"/* Number of colors: 16 */\n");
  }else{
    fprintf(hFile,"/* Number of colors: 256 */\n");
  }
  fprintf(hFile,"\n");
  
  if(globals.m_writeTiles){
    fprintf(hFile,"#define %s_NR_OF_TILES %u\n",upperCaseDataName.c_str(),tileList.size());
    fprintf(hFile,"#define %s_TILESIZE_X %u\n",upperCaseDataName.c_str(),globals.m_tileXSize);
    fprintf(hFile,"#define %s_TILESIZE_Y %u\n",upperCaseDataName.c_str(),globals.m_tileYSize);
    if(globals.m_colors16){
      fprintf(hFile,"#define %s_TILESIZE %u\n",upperCaseDataName.c_str(),(globals.m_tileXSize/4)*globals.m_tileYSize);
    }else{
      fprintf(hFile,"#define %s_TILESIZE %u\n",upperCaseDataName.c_str(),(globals.m_tileXSize/2)*globals.m_tileYSize);
    }
  }
  
  if(globals.m_writeMap){
    fprintf(hFile,"#define %s_MAPSIZE_X %u\n",upperCaseDataName.c_str(),screenXSize/globals.m_tileXSize);
    fprintf(hFile,"#define %s_MAPSIZE_Y %u\n",upperCaseDataName.c_str(),screenYSize/globals.m_tileYSize);
    fprintf(hFile,"#define %s_MAPSIZE %u\n",upperCaseDataName.c_str(),(screenXSize/globals.m_tileXSize)*(screenYSize/globals.m_tileYSize));
  }
  if(globals.m_writePalette){
    if(globals.m_colors16){
      fprintf(hFile,"#define %s_PALETTE_COLORS 16\n",upperCaseDataName.c_str());
    }else{
      fprintf(hFile,"#define %s_PALETTE_COLORS 256\n",upperCaseDataName.c_str());
    }
  }
  fprintf(hFile,"\n");
  //Tiles
  if(globals.m_writeTiles){
    fprintf(hFile,"extern ");
    if(globals.m_constTiles){
      fprintf(hFile,"const ");
    }
    if(globals.m_colors16){
      fprintf(hFile,"unsigned short %sTileData[%u][%u];\n",dataName.c_str(),tileList.size(),(globals.m_tileXSize/4)*globals.m_tileYSize);
    }else{
      fprintf(hFile,"unsigned short %sTileData[%u][%u];\n",dataName.c_str(),tileList.size(),(globals.m_tileXSize/2)*globals.m_tileYSize);
    }
  }
  //Map
  if(globals.m_writeMap){
    fprintf(hFile,"extern ");
    if(globals.m_constTiles){
      fprintf(hFile,"const ");
    }
    fprintf(hFile,"unsigned short %sMapData[%u];\n",dataName.c_str(),(screenXSize/globals.m_tileXSize)*(screenYSize/globals.m_tileYSize));
  }

  //Palette
  if(globals.m_writePalette){
    fprintf(hFile,"extern ");
    if(globals.m_constTiles){
      fprintf(hFile,"const ");
    }
    if(globals.m_colors16){
      fprintf(hFile,"unsigned short %sPaletteData[16];\n",dataName.c_str());
    }else{
      fprintf(hFile,"unsigned short %sPaletteData[256];\n",dataName.c_str());
    }
  }
  
  fprintf(hFile,"#endif //%s_H\n",upperCaseDataName.c_str());
  fclose(hFile);
  
}


void WriteCFile(std::string fileName,
                std::string dataName,
                std::string upperCaseDataName,
                std::list<unsigned char*> tileList,
                unsigned short *tileMap,
                std::list<_RGBColor*> palette,
                _PCXHeader *pcxHeader){


  unsigned screenXSize=pcxHeader->m_imageXMax-pcxHeader->m_imageXMin+1;
  unsigned screenYSize=pcxHeader->m_imageYMax-pcxHeader->m_imageYMin+1;
//  unsigned screenTotalBytes=screenXSize*screenYSize;
  
  unsigned char progress=0;
  FILE *cFile=fopen(fileName.c_str(),"w");

  fprintf(cFile,"/* Tile size:        %u*%u */\n",globals.m_tileXSize,globals.m_tileYSize);
  fprintf(cFile,"/* Number of tiles:  %u */\n",tileList.size());
  if(globals.m_colors16){
    fprintf(cFile,"/* Number of colors: 16 */\n");
  }else{
    fprintf(cFile,"/* Number of colors: 256 */\n");
  }
  fprintf(cFile,"\n");

  if(globals.m_writeTiles){
    if(globals.m_constTiles){
      fprintf(cFile,"const ");
    }
    if(globals.m_colors16){
      fprintf(cFile,"unsigned short %sTileData[%u][%u]={\n",dataName.c_str(),tileList.size(),(globals.m_tileXSize/4)*globals.m_tileYSize);
    }else{
      fprintf(cFile,"unsigned short %sTileData[%u][%u]={\n",dataName.c_str(),tileList.size(),(globals.m_tileXSize/2)*globals.m_tileYSize);
    }
  
    std::list<unsigned char*>::iterator t=tileList.begin();
    unsigned tileCounter=0;
    while(t!=tileList.end()){
      fprintf(cFile,"  //Tile %u\n  {\n    ",tileCounter);
      unsigned long counterY;
      for(counterY=0;counterY<globals.m_tileYSize;counterY++){

      
        if(globals.m_colors16){
          //16 colors palette
       
          unsigned long counterX;
          for(counterX=0;counterX<globals.m_tileXSize/4;counterX++){
            unsigned short word=0;
            unsigned char counter;
            for(counter=0;counter<4;counter++){
              unsigned char pixel=(*t)[(counterY*(globals.m_tileXSize))+(counterX*4)+counter];
              if(globals.m_invertPalette){
                if(globals.m_insertZeroTile){
                  if(t!=tileList.begin()){
                    pixel^=0xff;
                  }
                }else{
                  pixel^=0xff;
                }
              }

              pixel&=0x0f;
              word+=pixel<<(counter*4);
            }

            fprintf(cFile,"0x%04x,",word);
          }
        
        }else{
          unsigned long counterX;
          for(counterX=0;counterX<globals.m_tileXSize/2;counterX++){
            //256 colors palette
          
            unsigned short word=0;
            word=(*t)[(counterY*(globals.m_tileXSize))+(counterX*2)];
            word+=(*t)[(counterY*(globals.m_tileXSize))+(counterX*2)+1]<<8;
            
            if(globals.m_invertPalette){
              if(globals.m_insertZeroTile){
                if(t!=tileList.begin()){
                  word^=0xffff;
                }
              }else{
                word^=0xffff;
              }
            }
          
            fprintf(cFile,"0x%04x,",word);
          }
        }
        
        fprintf(cFile,"\n    ");
      }
      fseek(cFile,-4,SEEK_CUR);
  
      fprintf(cFile,"  },\n");
  
      tileCounter++;

      if(!globals.m_silet){
        unsigned char currProgress=(unsigned char)(((float)tileCounter/(float)tileList.size())*100.0);
        if(progress!=currProgress){
          progress=currProgress;
          printf("\rWriting file...(%u%%)",progress);
        }
      }
    
      t++;
    }
    fprintf(cFile,"};\n\n");
  }


  //Map
  if(globals.m_writeMap){
    if(globals.m_constTiles){
      fprintf(cFile,"const ");
    }
    fprintf(cFile,"unsigned short %sMapData[%u]={\n",dataName.c_str(),(screenXSize/globals.m_tileXSize)*(screenYSize/globals.m_tileYSize));
    unsigned short counterY;
    for(counterY=0;counterY<screenYSize/globals.m_tileYSize;counterY++){
      fprintf(cFile,"  ");
      unsigned short counterX;
      for(counterX=0;counterX<screenXSize/globals.m_tileXSize;counterX++){
        fprintf(cFile,"0x%04x,",tileMap[(counterY*(screenXSize/globals.m_tileXSize))+counterX]);
      }
      fprintf(cFile,"\n");
    }
    fprintf(cFile,"};\n\n");
  }
  

  
  if(globals.m_writePalette){
    //Palette
    if(globals.m_constTiles){
      fprintf(cFile,"const ");
    }
    if(globals.m_colors16){
      fprintf(cFile,"unsigned short %sPaletteData[16]={\n",dataName.c_str());
    }else{
      fprintf(cFile,"unsigned short %sPaletteData[256]={\n",dataName.c_str());
    }
    unsigned short paletteCounter;
    unsigned short numberOfColors;
    std::list<_RGBColor*>::iterator it;
  
    if(globals.m_colors16){
      numberOfColors=16;
    }else{
      numberOfColors=256;
    }

    if(globals.m_invertPalette){
      it=palette.end();
      it--;
    }else{
      it=palette.begin();
    }
    for(paletteCounter=0;paletteCounter<numberOfColors;paletteCounter++){
      _RGBColor *rgbColor=(*it);
    
      unsigned short color=(rgbColor->R&0xf8)>>3;
      color+=(rgbColor->G&0xf8)<<2;
      color+=(rgbColor->B&0xf8)<<7;
      fprintf(cFile,"  0x%04x,\n",color);

      if(globals.m_invertPalette){
        it--;
      }else{
        it++;
      }
    }
    fprintf(cFile,"};\n\n");
  }
  
  
  
  fclose(cFile);

  if(!globals.m_silet){
    printf("\rWriting file...(100%%)\n");
  }
}

void WritePCXFile(std::string fileName,
                  std::list<unsigned char*> tileList,
                  std::list<_RGBColor*> palette,
                  _PCXHeader *pcxHeader){

  signed short s16=0;
  signed char  s8=0;
  signed char  buff[769];
  
  FILE *pcxFile=fopen(fileName.c_str(),"wb");

  //write header

  _PCXHeader header;
  header.m_manufacturer=10;
  header.m_version=5;
  header.m_encoding=1;
  header.m_bitsPerPixel=8;
  header.m_imageXMin=0;
  header.m_imageYMin=0;
  header.m_imageXMax=globals.m_tileXSize-1;
  header.m_imageYMax=globals.m_tileYSize*tileList.size()-1;
  header.m_HDpi=globals.m_tileXSize;
  header.m_VDpi=globals.m_tileYSize*tileList.size();
  unsigned short counter;
  std::list<_RGBColor*>::iterator color;
  if(globals.m_invertPalette){
    color=palette.end();
    color--;
  }else{
    color=palette.begin();
  }
  for(counter=0;counter<16;counter++){
    header.m_colorMap[counter*3]=(*color)->R;
    header.m_colorMap[(counter*3)+1]=(*color)->G;
    header.m_colorMap[(counter*3)+2]=(*color)->B;
    if(globals.m_invertPalette){
      color--;
    }else{
      color++;
    }
  }
  header.m_reserved=0;
  header.m_nPlanes=1;
  header.m_bytesPerLine=globals.m_tileXSize;
  header.m_paletteInfo=1;
  header.m_hScreenSize=0;
  header.m_vScreenSize=0;
  memset(header.m_filler,0,54);

  fwrite(&header,128,1,pcxFile);

  std::list<unsigned char*>::iterator tile=tileList.begin();
  while(tile!=tileList.end()){
    
    for(counter=0;counter<globals.m_tileYSize;counter++){
      
      unsigned char lineCounter=0;
     
      
      while(lineCounter!=globals.m_tileXSize){
        
        unsigned char value=0;
        value=(*tile)[(counter*globals.m_tileXSize)+lineCounter];
        if(globals.m_invertPalette){
          value^=0xff;
        }

        unsigned char run=0;
        unsigned char current=0;
        do{
          current=(*tile)[(counter*globals.m_tileXSize)+lineCounter+run];
          if(globals.m_invertPalette){
            current^=0xff;
          }
          run++;
        }while((current==value)&&(run!=63)&&((run+lineCounter)<globals.m_tileXSize));


        if(current!=value){
          run--;
        }
        
        if(run==1){
          lineCounter++;
          if((value&0xc0)==0xc0){
            run=0xc1;
            fwrite(&run,1,1,pcxFile);
            fwrite(&value,1,1,pcxFile);
            run=0;
          }else{
            fwrite(&value,1,1,pcxFile);
            run=0;
          }
        }else{
          lineCounter+=run;
          run|=0xc0;
          fwrite(&run,1,1,pcxFile);
          fwrite(&value,1,1,pcxFile);
        }
      }
    }


    tile++;
  }


  
  memset(buff,0,769);
  if(globals.m_invertPalette){
    color=palette.end();
    color--;
  }else{
    color=palette.begin();
  }
  buff[0]=12;
  for(counter=0;counter<256;counter++){
    buff[(counter*3)+1]=(*color)->R;
    buff[(counter*3)+2]=(*color)->G;
    buff[(counter*3)+3]=(*color)->B;
    if(globals.m_invertPalette){
      color--;
    }else{
      color++;
    }
  }
  fwrite(buff,769,1,pcxFile);
  
  

  fclose(pcxFile);
}



