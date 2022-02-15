#include "vertsys.h"

void Lock(fd) 
int fd;
{
   lseek(fd,0L,0);
   if ( lockf(fd,F_LOCK,0L) == -1)
   printf("cant LOCK_EX\n");
}


void UnLock(fd)
int fd;
{
    lseek(fd,0L,0);
    if ( lockf(fd,F_ULOCK,0L) == -1)
        printf("cant LOCK_UN\n");
}
/* ------------------------------------------------------------------------ */

void CreateRequest() {
   creat("request",0666);
}

void WaitForServerIsReady() { 
     while (unlink("ready") == -1) sleep(1); 
}

void DisplayResponse() {
    FILE *stream; char c;
    stream = fopen("response","r");
    while (!feof(stream)) {   	
          if((c=fgetc(stream)) >=' ') printf("%c",c);
    }
    printf("\n");
    fclose(stream);
    unlink("response");
}

/* ------------------------------------------------------------------------ */
main (argc, argv) int argc; char *argv[]; {
int fd;

    if ( (fd=open("medium",O_RDWR)) == -1) {
       printf("Die Datei \"medium\" ist nicht vorhanden - Serverfehler!\n");
       exit(-1);
    }
    Lock(fd);
    CreateRequest();
    WaitForServerIsReady();
    DisplayResponse();
    UnLock(fd);
}

