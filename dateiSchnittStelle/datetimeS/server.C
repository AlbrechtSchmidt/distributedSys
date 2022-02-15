#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>

#include "KommuDIR.h" 

int pfd_M[2],pfd_S[2], nread;

void no_block(int p)
{ 
  int flags;
  if (  (flags=fcntl(p, F_GETFL, 0)) == -1 )
             perror("\n fcntl 1\n");
  if ( fcntl(p, F_SETFL, flags | O_NDELAY ) == -1)
             perror("\n fcntl 2\n");
}
 

int write_alive(char *msg, int p)
{ 
  int ret;
  int t=1;
  do
  { 
    t=t+1;
    ret = write(p, msg, sizeof(msg));
  }
  while( (t<5) && (ret == -1)) ;
  if ( ret == -1 ) perror("\n Write to Pipe - Error \n ");
  return ret;
}

int is_alive(char *msg, int p)
{ 
  int nread;
  char s[100];
  int t=1;
  do
  { 
    if (t>1) sleep(1); 
    t=t+1;
    nread=read(p, s, sizeof(s));
  }
  while( (t<10) && (nread ==0 )) ;
  
  if (nread == -1)
  {  
     perror("\n Pipe Read - Error \n"); return 0;
  } 
 // printf("\n  in is_alive: >>%s<< \n", s); 
  return strspn(s, msg) ;
}
 


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
   IdStr[0]='\0'; 
   dirp = opendir( KommuDIR );
     
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
   
   closedir( dirp );

   return IdStr;
}   

int Sohn(int pid)
{
  char IdStr[20];
  int ret, mypid;
  mypid=getpid();
  printf("\n Server %i on !\n",mypid); 
  chdir(KommuDIR);
  do
  {  
     write_alive("1", pfd_M[1]);
     ret = is_alive("1" , pfd_S[0]);
     
     if (!ret) return 2;

     strcpy(IdStr,next_Req_ID());
     if (!strcmp(IdStr, "kill"))
     {
       remove_Request(IdStr);
       printf("\nServer %i off !!! \n", mypid);
       printf("\nKill ControlServer: pid %i !!!\n\n",pid);
       kill(pid,15);
       return 0;
     }
     else
     if (!strlen(IdStr)==0)
     {
        printf("\n-----------------------------\n"); 
        printf("\nRequest form Client >>%s<<\n", IdStr); 
        write_Time(IdStr, Read_SysTime());
        write_Ready(IdStr);
        remove_Request(IdStr);
     }
     
     sleep(1);

  }
  while(1==1);
}

int Vater()
{
  int ret,i, pid;
  char  ja[1];
  pid=getpid();
  printf("\n ControlServer %i is ON !!! \n",pid);
    
  do 
  {
     write_alive("1", pfd_S[1]);
   
     ret = is_alive("1" , pfd_M[0]);
    /* printf("\n ret = %i  \n", ret); */
 
 
     printf("\n") ; 
   //if (ret) printf("\n"); //\n 1 is alive !!\n");
     //  else printf("\n") ; // \n 1 is not  alive !!\n");
 
  }
  while(ret);
 
  printf("\n Server is killed !!!\n");
  return 1;
}




int main()
{
  int pid,r,Vater_pid;
  char  ja[1];

 
  if (pipe(pfd_M) == -1)
  {
    printf("\n Fehler beim oeffnen der Pipe !!! \n");
    return -2;
  }
  
  if (pipe(pfd_S) == -1)
  {
    printf("\n Fehler beim oeffnen der Pipe !!! \n");
    return -2;
  }
 
  no_block(pfd_M[0]);
  no_block(pfd_M[1]);
  no_block(pfd_S[0]);
  no_block(pfd_S[1]);
  
do{
  
    printf(" \n fork() !!! \n");
    Vater_pid=getpid(); 
    pid=fork();
    switch (pid)
    {
      case -1: printf("\n Fehler bei fork() \n");
               return -1;
      case 0:  r=Sohn(Vater_pid);
               break;
      default: r=Vater(); 
    }
  }
  while((r==1)||(r==2));
}
