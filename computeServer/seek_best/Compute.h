#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define TRUE      1
#define GET_MSG "getLoad"
#define EXEC_MSG "remoteExec"
#define CLOSE_MSG "bye"
#define KILL_MSG  "kill"

#define EOT ">EOT<"
#define KEY 130 
#define S_PORT 12322 

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
