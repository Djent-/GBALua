#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main(void){
  unsigned long buildCnt=0x000000;
  
  FILE *hFile=fopen("BuildCnt.h","rt");
  if(hFile!=NULL){
    bool done=false;
    char str[100];
    while(!done){
      if(fgets(str,100,hFile)==NULL){
        done=true;
      }else{
        if(strstr(str,"#define BUILD_COUNT ")!=NULL){
          buildCnt=atol(str+20)+1;
        }
      }
    }
    fclose(hFile);
  }
  
  hFile=fopen("BuildCnt.h","wt");
  
  fprintf(hFile,"#ifndef BUILDCNT_H\n");
  fprintf(hFile,"#define BUILDCNT_H\n\n");

  fprintf(hFile,"#define BUILD_COUNT %lu\n\n",buildCnt);
  
  fprintf(hFile,"#endif\n\n");
  
  fclose(hFile);
}
