#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libelf.h>
  

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
    sleep(1); 
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
    t=t+1;
    sleep(1); 
    nread=read(p, s, sizeof(s));
  }
  while( (t<5) && (nread == -1)) ;

  if (nread == -1) 
  {
     perror("\n Pipe Read - Error \n"); return 0;
  }
/*  printf("\n  in is_alive: >>%s<< \n", s); */
  return strspn(s, msg) ; 
} 



void print_son()
{
  int i;
  printf("\n hier ist der Sohn !!! \n");
 
  do
  {
    printf("\n son \n");
    write_alive("1", pfd_M[1]);
    sleep(1);
  }
  while(1==1);
   
}

int print_father()
{
  int ret,i;
  char  ja[1];
  printf("\n hier ist der Vater !!! \n");
  sleep(1);
  
  do 
  {
   
     ret = is_alive("1" , pfd_M[0]); 
    /* printf("\n ret = %i  \n", ret); */
  
     sleep(1); 
     if (ret) printf("\n 1 is alive !!\n");
      else printf("\n 1 is not  alive !!\n");
 
  
  }
  while(ret); 

  printf("\n nochmal starten ? (j/n) \n");
  gets(ja);

  if (ja[0]=='j') return 1; else return 0;
}

int main()
{
  int pid,r;
  printf("\n ***** Start ---> !!!!! \n");

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


  do
  {
    printf(" \n fork() !!! \n"); 
    pid=fork();
    switch (pid) 
    {
      case -1: printf("\n Fehler bei fork() \n");
               return -1;
      case 0:  close(pfd_S[0]); /* Vater write schliessen */
               close(pfd_S[0]); /* Sohn read schliessen */
               print_son();
               return 0;
      default: close(pfd_S[1]); /* Sohn write schliessen */
               close(pfd_S[1]); /* Vater read schliessen */ 
                r=print_father();
   }  
  printf("\n r= %i \n", r);
  }
  while(r==1); 

  printf("\n Hier nochmal im Vater-Programm !!! \n");
  return 1;
} 
