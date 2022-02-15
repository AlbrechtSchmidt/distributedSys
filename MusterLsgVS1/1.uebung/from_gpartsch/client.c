#include "vertsys.h"

/* ------------------------------------------------------------------------ */
void Lock1()   {while(unlink("waiting")==-1) sleep(1);}

void UnLock1() {creat("waiting",0666);}
/* ------------------------------------------------------------------------ */
void Lock2()   { 
   int fd; char tempfile[30];
   sprintf(tempfile,"LOCK%d",getpid());
   fd=creat(tempfile,0666);
   close(fd);
   while (link(tempfile,LOCKFILE)<0); sleep(1);
   unlink(tempfile);
}

void UnLock2() {unlink(LOCKFILE);}
/* ------------------------------------------------------------------------ */
void Lock3() {
   int fd;
   while ((fd=creat(LOCKFILE,0))<0); sleep(1);
   close(LOCKFILE);
}

void UnLock3() { unlink(LOCKFILE); }
/* ------------------------------------------------------------------------ */
void Lock4() {
   int fd;
   while ( (fd=open(LOCKFILE,O_RDWR|O_CREAT|O_EXCL,0666))<0) sleep(1);
   close(fd);
}

void UnLock4() {unlink(LOCKFILE);}
/* ------------------------------------------------------------------------ */
void CreateRequest() {creat("request",0666);}

void WaitForServerIsReady() { while (unlink("ready") == -1) sleep(1); }

void DisplayResponse() {
    FILE *stream; char c;
    stream = fopen("response","r");
    while (!feof(stream)) {	
           if ((c=fgetc(stream)) >= ' ') printf("%c",c);
    }
    printf("\n");
    fclose(stream);
    unlink("response");
}
/* ------------------------------------------------------------------------ */
main (argc, argv) int argc; char *argv[]; {
    if (argc > 1) chdir(argv[1]);
    Lock1(); /* Lock2(); Lock3(); Lock4(); */
    CreateRequest();
    WaitForServerIsReady();
    DisplayResponse();
    UnLock1(); /* UnLock2(); UnLock3(); UnLock4(); */
}
