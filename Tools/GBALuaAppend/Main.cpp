#include <stdio.h>
#include <string.h>
#include <list>

typedef struct{
  char            *name;
  unsigned        length;
  unsigned char   *data;
} fileInfo;

std::list<fileInfo*>  fileInfoList;
unsigned char         *masterFile=NULL;
unsigned              masterFileSize=0;
char                  outName[256]="\0";
bool                  multiboot=false;
bool                  foundLuaCode=false;

bool processFiles(int argc,char *argv[]){
  unsigned short counter;
  for(counter=1;counter<argc;counter++){
    if(argv[counter][0]=='-'){
      if(stricmp(argv[counter],"-m")==0){
        if(masterFile==NULL){
          FILE *file=fopen(argv[counter+1],"rb");
          if(file!=NULL){
            fseek(file,0,SEEK_END);
            masterFileSize=ftell(file);
            masterFile=new unsigned char[masterFileSize];
            fseek(file,0,SEEK_SET);
            fread(masterFile,masterFileSize,1,file);
            fclose(file);
          }else{
            printf("Warning: couldn't open masterfile: %s\n",argv[counter+1]);
          }
          counter++;
        }else{
          printf("Warning: Allready opened a masterfile (ignoring %s)\n",argv[counter+1]);
        }
      }
      if(stricmp(argv[counter],"-o")==0){
        strcpy(outName,argv[counter+1]);
        counter++;
      }
      if(stricmp(argv[counter],"-mb")==0){
        multiboot=true;
      }
    }else{
      FILE *file=fopen(argv[counter],"rb");
      if(file!=NULL){
        fseek(file,0,SEEK_END);
      
        fileInfo *fi=new fileInfo;
        fi->length=ftell(file);
        char *name;
        char *tmp2;
        char tempName[256];
        strcpy(tempName,argv[counter]);
        if(strrchr(tempName,'.')!=NULL){
          strcpy(strrchr(tempName,'.'),"\0");
        }
      
        if(strrchr(tempName,'\\')!=NULL){
          char tmpName[256];
          strcpy(tmpName,strrchr(tempName,'\\')+1);
          tmp2=new char[strlen(tmpName)+1];
          strcpy(tmp2,tmpName);
        }else{
          tmp2=new char[strlen(tempName)+1];
          strcpy(tmp2,tempName);
        }
      
        if(strrchr(tmp2,'/')!=NULL){
          char tmpName[256];
          strcpy(tmpName,strrchr(tmp2,'/')+1);
          name=new char[strlen(tmpName)+1];
          strcpy(name,tmpName);
        }else{
          name=new char[strlen(tmp2)+1];
          strcpy(name,tmp2);
        }

        delete tmp2;

        if(strcmp(name,"LuaCode")==0){
          foundLuaCode=true;
        }
      
        fi->name=name;
        unsigned char *data=new unsigned char[fi->length];
        fseek(file,0,SEEK_SET);
        fread(data,fi->length,1,file);
        fclose(file);
        fi->data=data;

        fileInfoList.push_back(fi);
      }else{
        printf("Warning: couldn't open file: %s\n",argv[counter]);
      }
    }
  }

  if(masterFile==NULL){
    printf("Error: No master file opened\n");
    std::list<fileInfo*>::iterator  t=fileInfoList.begin();
    while(t!=fileInfoList.end()){
      delete (*t)->data;
      delete (*t)->name;
      delete (*t);
      t++;
    }
    return false;
  }

  if(fileInfoList.size()==0){
    printf("Error: No appending files given\n");
    delete masterFile;
    return false;
  }

  if(!foundLuaCode){
    printf("Warning: Couldn't find any luacode added. The lua code has to be \n");
    printf("         namned 'LuaCode.<ext>' where LuaCode is case sensitive \n");
    printf("         and <ext> can be whatever. The binary will be created but\n");
    printf("         won't start properly by GBALuaCore.\n");
  }

  std::list<fileInfo*>::iterator  t=fileInfoList.begin();
  unsigned    dataSize=0;
  unsigned    hdrSize=4;
  while(t!=fileInfoList.end()){
    dataSize+=(*t)->length+(4-((*t)->length%4));
    hdrSize+=strlen((*t)->name)+(4-(strlen((*t)->name)%4))+12;
    t++;
  }

  unsigned char *outBuffer;
  if(multiboot){
    outBuffer=new unsigned char[0x40000];
  }else{
    int size=masterFileSize+(4-(masterFileSize%4))+dataSize+hdrSize;
    outBuffer=new unsigned char[masterFileSize+(4-(masterFileSize%4))+dataSize+hdrSize];
  }
  memcpy(outBuffer,masterFile,masterFileSize);
  delete masterFile;
  
  int currDataPos;
  int currHdrPos;
  int startPos;
  
  if(multiboot){
    currDataPos=0x40000-(dataSize+hdrSize);
    currHdrPos=0x40000-(hdrSize);
    startPos=currHdrPos;
    
    if(currDataPos<=masterFileSize){
      printf("Error: Appended data is too big! Data will overwrite the code of the masterfile\n");
      
      std::list<fileInfo*>::iterator  t=fileInfoList.begin();
      while(t!=fileInfoList.end()){
        delete (*t)->data;
        delete (*t)->name;
        delete (*t);
        t++;
      }
      
      return false;
    }
  }else{
    startPos=masterFileSize+(4-(masterFileSize%4))+4;
    currHdrPos=startPos;
    currDataPos=currHdrPos+hdrSize-4;
  }
  
  t=fileInfoList.begin();
  while(t!=fileInfoList.end()){
    memcpy(outBuffer+currDataPos,(*t)->data,(*t)->length);

    delete (*t)->data;

    unsigned len=strlen((*t)->name)+(4-strlen((*t)->name)%4);
    memcpy(outBuffer+currHdrPos,&len,4);
    currHdrPos+=4;
    strcpy((char*)outBuffer+currHdrPos,(*t)->name);
    currHdrPos+=strlen((*t)->name)+(4-(strlen((*t)->name)%4));
    memcpy(outBuffer+currHdrPos,&(*t)->length,4);
    currHdrPos+=4;
    memcpy(outBuffer+currHdrPos,&currDataPos,4);
    currHdrPos+=4;

    currDataPos+=(*t)->length+(4-((*t)->length%4));

    delete (*t)->name;

    delete (*t);

    t++;
  }

  if(multiboot){
    memcpy(outBuffer+0x40000-4,&startPos,4);
  }else{
    memcpy(outBuffer+masterFileSize+(4-masterFileSize%4),&startPos,4);
  }

  if(strlen(outName)==0){
    if(multiboot){
      strcpy(outName,"GBALuaApp.mb.gba");
    }else{
      strcpy(outName,"GBALuaApp.gba");
    }
  }  
    
  FILE *file=fopen(outName,"wb");
  if(file!=NULL){
    if(multiboot){
      fwrite(outBuffer,0x40000,1,file);
    }else{
      fwrite(outBuffer,masterFileSize+(4-(masterFileSize%4))+dataSize+hdrSize+4,1,file);
    }
    delete outBuffer;
    fclose(file);
  }else{
    printf("Error: Couldn't open outfile: %s\n",outName);
    delete outBuffer;
    return false;
  }


  return true;
}

int main(int argc, char *argv[]){

  if(argc<2){
    printf("GBALuaAppend by superike (dan@netbabyworld.com) 2002\n\n");
    
    printf("  Usage: GBALuaAppend.exe <-o outfile> <-mb> -m masterfile file1 <file2...>\n\n");
    
    printf("If no output file is given (the -o switch) the output will default\n");
    printf("to GBALuaApp.gba (GBALuaApp.mb.gba if the -mb switch is given). The\n");
    printf("masterfile and the outfile can be the same but it is not encuraged\n");
    printf("because that will cause the masterfile to get overwritten\n\n");
    return 0;
  }

  if(processFiles(argc,argv)==false){
    return 1;
  }


  return 0;
}
