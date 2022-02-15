#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "KommuDIR.h"

#define TIMEOUT 100

/* erstellt das Request-File (leeres File)     
   das dem Server signalisiert, dass ein Client einen
   Request fordert. Die Sender-Adresse (Client-Nr)
   steht im Dateinamen. Die Zugriffsrechte sind so gesetzt
   dass der Server die Datei loeschen kann.              */
void write_Request(int id)
{
  FILE *file_nr;
  char File_ID_str[30];
  sprintf(File_ID_str,"Request.%i", id);
  file_nr = fopen(File_ID_str, "w+");
  fclose(file_nr);
  chmod(File_ID_str, S_IRWXO | S_IRWXU);
  printf("\nClient writes file %s ", File_ID_str );
} 

/* Prueft ob ein Ready-File (die Antwort des Servers
   auf die Anfrage) mit der eigenen Client-ID existiert. */
int exist(int id)
{
  char FileName[30];
  sprintf(FileName,"Ready.%i", id);
  if (access(FileName, 0) == -1) return 0;
                                 else return 1;
}

/* loescht das Ready-File mit der eigenen Client-ID */
void remove_Ready(int id)
{
  char FileName[30];
  sprintf(FileName,"Ready.%i", id);
  remove(FileName);
  printf("\nClient removes file %s ", FileName );
}

/* loescht das Time-File mit der eigenen Client-ID */
void remove_Time(int id)
{
  char FileName[30];
  sprintf(FileName,"Time.%i", id);
  remove(FileName);
  printf("\nClient removes file %s ", FileName );
}

/* loescht das Request-File mit der eigenen Client-ID */
void remove_Request(int id)
{
  char FileName[30];
  sprintf(FileName,"Request.%i", id);
  remove(FileName);
  printf("\nClient removes file %s ", FileName );
}

/* liest die Zeit aus dem Time-File mit der 
   eigenen Client-ID                          */
void read_time_file(int id, char *TStr)
{
  FILE *file_nr;
  char FileName[30];
  sprintf(FileName,"Time.%i", id);
  file_nr = fopen(FileName, "rt");
  fgets(TStr, 20, file_nr);
  fclose(file_nr);
}

int main()
{
    int P_ID ;
    char TimeStr[20];
    char weiter[2]; 
    int timecounter = 0;
    P_ID = getpid();
    printf("\nClient %i is on !\n", P_ID); 
    chdir(KommuDIR); 
    do 
    {
       printf("\n-----------------------------\n"); 
       write_Request(P_ID);
       while (!exist(P_ID) && (timecounter <= TIMEOUT))
       {
         timecounter ++;
         sleep(1); //Verzoegerung: eine Sekunde
       }
       if (timecounter > TIMEOUT) 
       {
         printf( "\n\n  TimeOutError !!! \n");
         remove_Request(P_ID);
         printf("\n\n-----------------------------\n"); 
         return -1;
       }
       read_time_file(P_ID,TimeStr);  // Zeit einlesen !!!
       printf("\n\n==> aktuelle Zeit : %s <==\n", TimeStr); 
       remove_Ready(P_ID);
       remove_Time(P_ID);
       printf("\n\n??? weiteren Request schicken ? (j/n) : ");
       gets(weiter); 
    }
    while(weiter[0]=='j');
    printf("\n-----------------------------\n"); 
    printf("\nClient %i is off !\n", P_ID); 
    return 0;
}
 
