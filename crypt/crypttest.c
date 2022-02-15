#include <stdio.h>
#include <string.h>
#include <des_crypt.h>


void encode(char key, char * code, char * text)
{
  int i;
  for(i=0;i < strlen(text);i++)
  {
    code[i]=key ^ text[i];
  }
  code[strlen(text)]='\0';
}

void decode(char key, char * code, char * text)
{
  int i;
  for(i=0;i < strlen(code);i++)
  {
    text[i]=key ^ code[i];
  }
  text[strlen(code)]='\0';
}


