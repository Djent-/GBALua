#ifdef WIN32
#pragma warning (disable:4786) // truncated debug info names
#endif

#include "DecodePCX.h"
#include "Params.h"

bool HandlePCXHeader(FILE *fileHandle,_PCXHeader *pcxHeader){

  fseek(fileHandle,0,SEEK_SET);
  if(fread(pcxHeader,sizeof(_PCXHeader),1,fileHandle)!=1){
    if(!globals.m_totalSilence){
      fprintf(stderr,"File couldn't be read\n");
    }
    globals.m_retValue=2;
    return false;
  }

  if(!globals.m_silet){
    if(globals.m_showFileHeader){
        
      printf("PCX Header:\n");
    
      //Manufacturer
      if(pcxHeader->m_manufacturer==10){
        printf("\tManufacturer:   ZSoft .pcx\n");
      }else{
        printf("\tManufacturer:   Unknown (%u)\n",pcxHeader->m_manufacturer);
      }

      //Version
      switch(pcxHeader->m_version){
      case 0:
        printf("\tVersion:        Version 2.5 of PC Paintbrush\n");
        break;
      case 2:
        printf("\tVersion:        Version 2.8 with palette\n");
        break;
      case 3:
        printf("\tVersion:        Version 2.8 without palette\n");
        break;
      case 4:
        printf("\tVersion:        PC Paintbrush for Windows\n");
        break;
      case 5:
        printf("\tVersion:        Version 3.0\n");
        break;
      default:
        printf("\tVersion:        Unknown (%u)\n",pcxHeader->m_version);
        break;
      }
        
      //Encoding
      if(pcxHeader->m_encoding==1){
        printf("\tEncoding:       .PCX run length encoding\n");
      }else{
        printf("\tEncoding:       Unknown (%u)\n",pcxHeader->m_encoding);
      }
        
      //Bits Per Pixel
      printf("\tBits Per Pixel: %u bits\n",pcxHeader->m_bitsPerPixel);
        
      printf("\tWindow X Min:   %u\n",pcxHeader->m_imageXMin);
      printf("\tWindow Y Min:   %u\n",pcxHeader->m_imageYMin);
      printf("\tWindow X Max:   %u\n",pcxHeader->m_imageXMax);
      printf("\tWindow Y Max:   %u\n",pcxHeader->m_imageYMax);
      printf("\tHDpi:           %u\n",pcxHeader->m_HDpi);
      printf("\tVDpi:           %u\n",pcxHeader->m_VDpi);

      printf("\tReserved:       %u\n",pcxHeader->m_reserved);
      printf("\tNr Of Planes:   %u\n",pcxHeader->m_nPlanes);
      printf("\tBytes Per Line: %u\n",pcxHeader->m_bytesPerLine);
      switch(pcxHeader->m_paletteInfo){
      case 1:
        printf("\tPalette Info:   Color\n");
        break;
      case 2:
        printf("\tPalette Info:   Grey scale\n");
        break;
      default:
        printf("\tPalette Info:   Unknown (%u)\n",pcxHeader->m_paletteInfo);
        break;
      }
      printf("\tH Screen Size:  %u\n",pcxHeader->m_hScreenSize);
      printf("\tV Screen Size:  %u\n",pcxHeader->m_vScreenSize);

      unsigned char counter=0;
      printf("\tColorMap:\n\t");
      for(counter=0;counter<48;counter++){
        printf("%02x ",pcxHeader->m_colorMap[counter]);
        if(counter%20==19){
          printf("\n\t");
        }
      }
      printf("\n");

      printf("\tFiller:\n\t");
      for(counter=0;counter<54;counter++){
        printf("%02x ",pcxHeader->m_filler[counter]);
        if(counter%20==19){
          printf("\n\t");
        }
      }
      printf("\n");
    }
  
    //check the header
  
    //manufacturer
    if(pcxHeader->m_manufacturer!=10){
      if(!globals.m_totalSilence){
        fprintf(stderr,"Error: Manufacturer: Unknown (%u)\n",pcxHeader->m_manufacturer);
        fprintf(stderr,"Please send this file to dan@netbabyworld.com\n");
      }
      globals.m_retValue=6;
      return false;
    }
  
    //version
    switch(pcxHeader->m_version){
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
      break;
    default:
      if(!globals.m_totalSilence){
        fprintf(stderr,"Error: Unknown version: %u\n",pcxHeader->m_version);
        fprintf(stderr,"Please send this file to dan@netbabyworld.com\n");
      }
      globals.m_retValue=7;
      return false;
      break;
    }
  
    if(pcxHeader->m_encoding!=1){
      if(!globals.m_totalSilence){
        fprintf(stderr,"Error: Unknown encoding: %u\n",pcxHeader->m_encoding);
        fprintf(stderr,"Please send this file to dan@netbabyworld.com\n");
      }
      globals.m_retValue=8;
      return false;
    }
  
    if(pcxHeader->m_nPlanes!=1){
      if(!globals.m_totalSilence){
        fprintf(stderr,"Error: Can not decode more than 1 plane yet (%u)\n",pcxHeader->m_nPlanes);
        fprintf(stderr,"Please send this file to dan@netbabyworld.com\n");
      }
      globals.m_retValue=9;
      return false;
    }
  
    switch(pcxHeader->m_paletteInfo){
    case 1:
    case 2:
      break;
    default:
      if(!globals.m_totalSilence){
        fprintf(stderr,"Error: Unknown palette info: %u\n",pcxHeader->m_paletteInfo);
        fprintf(stderr,"Please send this file to dan@netbabyworld.com\n");
      }
      globals.m_retValue=10;
      return false;
      break;
    }
  }

  return true;
}














bool DecodePCXData(FILE *fileHandle,_PCXHeader *pcxHeader,unsigned char *screenData){

  if(!globals.m_silet){
    printf("Decoding file...(0%)");
  }
  
  //this is just to find out how much data there is in the file
  fseek(fileHandle,0,SEEK_END);
  long dataSize=ftell(fileHandle);
  dataSize-=128; //- header
  if(pcxHeader->m_version==5){
    dataSize-=768;  //asume that there is a palette at the end
  }
  
  //set filepos to start of data
  fseek(fileHandle,128,SEEK_SET);

  //alloc data
  unsigned char *fileData=new unsigned char[dataSize];
  if(fread(fileData,dataSize,1,fileHandle)!=1){
    if(!globals.m_totalSilence){
      fprintf(stderr,"File couldn't be read\n");
    }
    globals.m_retValue=2;
    return false;
  }

  //find the size of teh screen to alloc
  unsigned xSize=pcxHeader->m_imageXMax-pcxHeader->m_imageXMin+1;
  unsigned ySize=pcxHeader->m_imageYMax-pcxHeader->m_imageYMin+1;
  unsigned totalBytes=xSize*ySize;

  unsigned screenCounter=0;
  unsigned dataCounter=0;

  unsigned char progress=0;
  while(screenCounter!=totalBytes){
    if(screenCounter>=totalBytes){
      if(!globals.m_totalSilence){
        fprintf(stderr,"Error: Decoding error. File corrupt?\n");
        fprintf(stderr,"Please send this file to dan@netbabyworld.com\n");
      }
      globals.m_retValue=5;
      return false;
    }
    
    //check if top 2 bits are set    
    if((fileData[dataCounter]&0xc0)==0xc0){
      unsigned length=fileData[dataCounter]&0x3f;
      memset(screenData+screenCounter,fileData[dataCounter+1],length);
      screenCounter+=length;
      dataCounter+=2;

    }else{
      screenData[screenCounter]=fileData[dataCounter];
      screenCounter++;
      dataCounter++;
    }

    if(!globals.m_silet){
      unsigned char currProgress=(unsigned)(((float)screenCounter/(float)totalBytes)*100.0);
      if(progress!=currProgress){
        progress=currProgress;
        printf("\rDecoding file...(%u%%)",progress);
      }
    }
    
  }
  
  if(!globals.m_silet){
    printf("\rDecoding file...(100%%)\n");
  }
  
  //done with data so detete it
  delete [] fileData;
  
  return true;
}

