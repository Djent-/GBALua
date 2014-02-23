
#pragma warning (disable:4786) // truncated debug info names

#include "main.h"
#include "Params.h"
#include "DecodePCX.h"
#include "FileWrite.h"


std::list<unsigned char*> ExtractTiles(unsigned char *screenData,unsigned short *tileMap,_PCXHeader *pcxHeader){

  if(!globals.m_silet){
    printf("Extracting tiles...(0%)");
  }
  unsigned char progress=0;
  
  //set up a list for the tiles
  std::list<unsigned char*> tileList;

  if(globals.m_insertZeroTile){
    unsigned char *tile=new unsigned char[globals.m_tileXSize*globals.m_tileYSize];
    memset(tile,0,globals.m_tileXSize*globals.m_tileYSize);
    tileList.push_back(tile);
  }

  unsigned screenSizeX=pcxHeader->m_imageXMax-pcxHeader->m_imageXMin+1;
  unsigned screenSizeY=pcxHeader->m_imageYMax-pcxHeader->m_imageYMin+1;
  
  //go throgh each line of tiles
  unsigned tileCounterY;
  unsigned nrTilesY=screenSizeY/globals.m_tileYSize;
  for(tileCounterY=0;tileCounterY<nrTilesY;tileCounterY++){

    //go throgh each row of tiles
    unsigned tileCounterX;
    unsigned nrTilesX=screenSizeX/globals.m_tileXSize;
    for(tileCounterX=0;tileCounterX<nrTilesX;tileCounterX++){

      //alloc space for a new tile
      unsigned char *tile=new unsigned char[globals.m_tileXSize*globals.m_tileYSize];

      //eash line within the tile
      unsigned short counter;
      for(counter=0;counter<globals.m_tileYSize;counter++){
        //copy data
        
        memcpy(tile+(counter*globals.m_tileXSize),
          screenData+(((tileCounterY*globals.m_tileYSize)+counter)*screenSizeX)+(tileCounterX*globals.m_tileXSize),
          globals.m_tileXSize);
      }
      
      //here add tile to a list if not found in list allready
      
      bool equal=false;
      counter=0;
      if(globals.m_insertZeroTile){
        counter=1;
      }

      if(globals.m_optimizeTiles){
        std::list<unsigned char*>::iterator t=tileList.begin();
        while(t!=tileList.end()){
          if(memcmp(tile,(*t),globals.m_tileXSize*globals.m_tileYSize)==0){
            equal=true;
            t=tileList.end();
            tileMap[tileCounterY*nrTilesX+tileCounterX]=counter;
          }else{
            t++;
          }
          counter++;
        }
      }

      if(!equal){
        tileList.push_back(tile);
        tileMap[tileCounterY*nrTilesX+tileCounterX]=counter;
      }else{
        //delete the tile
        delete [] tile;
      }

      if(!globals.m_silet){
        unsigned char currProgress=(unsigned char)(((float)(tileCounterX+(tileCounterY*nrTilesX))/(float)(nrTilesY*nrTilesX))*100.0);
        if(progress!=currProgress){
          progress=currProgress;
          printf("\rExtracting tiles...(%u%%)",progress);
        }
      }
    }
  }
  
  if(!globals.m_silet){
    printf("\rExtracting tiles...(100%%)\n");
  }

  return tileList;
}







std::list<_RGBColor*> DecodePCXPalette(FILE *fileHandle,_PCXHeader *pcxHeader){
 
  if(!globals.m_silet){
    printf("Extracting palette...(0%%)");
  }
    
  
  unsigned char progress=0;
  std::list<_RGBColor*> palette;

  if(pcxHeader->m_version!=5){
    //the palette should be found within the header

    //store it in a nice list
    unsigned char counter;
    for(counter=0;counter<16;counter++){
      _RGBColor *color=new _RGBColor;
      color->R=pcxHeader->m_colorMap[(counter*3)];
      color->G=pcxHeader->m_colorMap[(counter*3)+1];
      color->B=pcxHeader->m_colorMap[(counter*3)+2];
      palette.push_back(color);
    
      if(!globals.m_silet){
        unsigned char currProgress=(unsigned char)(((float)counter/16.0)*100.0);
        if(progress!=currProgress){
          progress=currProgress;
          printf("\rExtracting palette...(%u%%)",progress);
        }
      }
      
    }

  }else{
    //seek back to the begining of the palette
    fseek(fileHandle,-769,SEEK_END);

    //check for a twelve
    unsigned char twelve;
    if(fread(&twelve,sizeof(unsigned char),1,fileHandle)!=1){
      if(!globals.m_totalSilence){
        fprintf(stderr,"File couldn't be read\n");
      }
      globals.m_retValue=2;
      return NULL;
    }
    if(twelve!=12){
      if(!globals.m_totalSilence){
        fprintf(stderr,"Palette extracting error. Error code: (%u)\n",twelve);
        fprintf(stderr,"Please send this file to dan@netbabyworld.com\n");
      }
      globals.m_retValue=4;
      return NULL;
    }

    //read back the palette from the file
    _RGBColor *paletteData=new _RGBColor[256];
    if(fread(paletteData,256*3,1,fileHandle)!=1){
      if(!globals.m_totalSilence){
        fprintf(stderr,"File couldn't be read\n");
      }
      globals.m_retValue=2;
      return NULL;
    }

    //store it in a nice list
    unsigned short counter;
    for(counter=0;counter<256;counter++){
      _RGBColor *color=new _RGBColor;
      color->R=paletteData[counter].R;
      color->G=paletteData[counter].G;
      color->B=paletteData[counter].B;
      palette.push_back(color);

      if(!globals.m_silet){
        unsigned char currProgress=(unsigned char)(((float)counter/256.0)*100.0);
        if(progress!=currProgress){
          progress=currProgress;
          printf("\rExtracting palette...(%u%%)",progress);
        }
      }
      
    }

    delete [] paletteData;
  }


  if(!globals.m_silet){
    printf("\rExtracting palette...(100%%)\n");
  }
    
  
  return palette;
}









void ProcessFiles(std::list<std::string> fileList){
  std::list<std::string>::iterator fileIterator=fileList.begin();

  while(fileIterator!=fileList.end()){
    
    FILE *fileHandle=fopen((*fileIterator).c_str(),"rb");
    if(fileHandle==NULL){
      if(!globals.m_totalSilence){
        fprintf(stderr,"Error: File \"%s\" couldn't be opened",(*fileIterator).c_str());
      }
      globals.m_retValue=1;
    }else{
      
      if(globals.m_generateDepend){
        //Find data name
        std::string dataName=(*fileIterator);
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
          dataName=dataName.substr(pos+1,dataName.size());
        }
      
        printf("%s.o: %s.c\\\n   %s.pcx\n",dataName.c_str(),dataName.c_str(),dataName.c_str());

      }else{
      
        if(!globals.m_silet){
          printf("Processing %s\n",(*fileIterator).c_str());
        }

        _PCXHeader *pcxHeader=new _PCXHeader;
      
        if(HandlePCXHeader(fileHandle,pcxHeader)){

          //find the size of teh screen to alloc
          unsigned xSize=pcxHeader->m_imageXMax-pcxHeader->m_imageXMin+1;
          unsigned ySize=pcxHeader->m_imageYMax-pcxHeader->m_imageYMin+1;

          if(globals.m_exportBitmap){
            globals.m_tileXSize=xSize;
            globals.m_tileYSize=ySize;
          }else{
            globals.m_tileXSize=8;
            globals.m_tileYSize=8;
          }
          
          
          unsigned totalBytes=xSize*ySize;
    
          //alloc the screen
          unsigned char *screenData=new unsigned char[totalBytes];
          if(DecodePCXData(fileHandle,pcxHeader,screenData)){

            unsigned short *tileMap=new unsigned short[(xSize/globals.m_tileXSize)*(ySize/globals.m_tileYSize)];
            std::list<unsigned char*> tileList=ExtractTiles(screenData,tileMap,pcxHeader);

            std::list<_RGBColor*> palette=DecodePCXPalette(fileHandle,pcxHeader);
            
            WriteOutFiles((*fileIterator),tileList,tileMap,palette,pcxHeader);

            //delete the map
            delete [] tileMap;

            //delete the palette
            std::list<_RGBColor*>::iterator p=palette.begin();
            while(p!=palette.end()){
              delete [] (*p);
              p++;
            }
            palette.clear();

            //delete all the tiles
            std::list<unsigned char*>::iterator t=tileList.begin();
            while(t!=tileList.end()){
              delete [] (*t);
              t++;
            }
            tileList.clear();
          }
          delete [] screenData;
        
        
        
          delete pcxHeader;
        }
    
        fclose(fileHandle);
      }
    }
    
    fileIterator++;
  }
}


int main(int argc,char *argv[]){
  InitGlobals();

  std::list<std::string> fileList=HandleParams(argc,argv);
  ProcessFiles(fileList);
  
  
  return globals.m_retValue;
}