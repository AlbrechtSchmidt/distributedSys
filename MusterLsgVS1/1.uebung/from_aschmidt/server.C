#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

#include "KommuDIR.h" 

/* erstellt das Ready-File (leeres File)
   das dem Client signalisiert, dass der Server fertig ist.
   Die Sender-Adresse (Client-Nr)
   steht im Dateinamen. Die Zugriffsrechte sind so gesetzt
   dass der Client die Datei loeschen kann.              */
void write_Ready(char *id)
{
  FILE *file_nr;
  char FileName[30];
  sprintf(FileName,"Ready.%s", id);
  file_nr = fopen(FileName, "w+");
  fclose(file_nr);
  chmod(FileName, S_IRWXO | S_IRWXU);
  printf("\nServer writes file %s \n", FileName );
}

/* schreibt das Time-File (Textstring mit Datum und Uhrzeit)
   das im Dateinamen die Clientnummr traegt.
   Die Zugriffsrechte sind so gesetzt
   dass der Client die Datei loeschen kann.              */
void write_Time(char *id, char *TimeStr)
{
  FILE *file_nr; 
  char FileName[30];
  sprintf(FileName,"Time.%s", id);
  file_nr = fopen(FileName, "wt");
  fputs(TimeStr, file_nr);
  fclose(file_nr);
  chmod(FileName, S_IRWXO | S_IRWXU);
  printf("\nServer writes file %s \n", FileName );
}

/* loescht das Request-File mit der eigenen Client-ID */
void remove_Request(char *id)
{
  char FileName[30];
  sprintf(FileName,"Request.%s", id);
  remove(FileName);
  printf("\nServer removes file %s \n", FileName );
}

/* liest die Systemzeit und gibt die in einem 
   String zurueck                              */
char *Read_SysTime()
{
   time_t t;
   time(&t);
   return ctime(&t);
}

/* sucht im Kommunikationsverzeichnis nach Anfragen
   (also noch Files der Form: Resquest.xxxx) und
   gibt die Identifikation als String zurueck     */
char *next_Req_ID()
{
   char Dir_entry[80];
   char IdStr[10];
   char isReqStr[10];
   int newReq=0;
   DIR *dirp;
   struct dirent *direntp;
 
   dirp = opendir( KommuDIR );
   do
   {
     rewinddir( dirp );
     while ( (direntp = readdir( dirp )) != NULL )
     {
         strcpy(Dir_entry,direntp->d_name);
         strncpy(isReqStr, Dir_entry, 7);
         isReqStr[7]='\0';
         if (!strcmp(isReqStr, "Request"))
         {
           strcpy(IdStr, Dir_entry+8);
           IdStr[strlen(Dir_entry)-8]='\0';
           newReq=1;
         } 
      }
      sleep(1); // eine Sekunde warten 
   }
   while(!newReq);
   closedir( dirp );

   return IdStr;
}   

int main()
{
  char IdStr[20]; 
  printf("\n Server on !\n"); 
  chdir(KommuDIR);
  do
  {  
     strcpy(IdStr,next_Req_ID());
     printf("\n-----------------------------\n"); 
     printf("\nRequest form Client %s\n", IdStr); 
     if (!strcmp(IdStr, "kill"))
     {
       remove_Request(IdStr);
       printf("\nServer off !!! \n");
       return 0;
     }
     else
     {
        write_Time(IdStr, Read_SysTime());
        write_Ready(IdStr);
        remove_Request(IdStr);
     }
  }
  while(1==1);
}
