#include <stdio.h>
  

int pfd[2], nread;


int write_alive(char *msg, int p)
{
  int ret;
  ret = write(p, msg, sizeof(msg))
  if ( ret == -1 ) perror("\n Write to Pipe - Error \n "); 
  return ret;
}

int is_alive(char *msg, int p) 
{
  int nread;
  char 
  nread=read(pfd[0], s, sizeof(s));
  if (nread == -1) 
  {
     perror("\n Pipe Read - Error \n"); return 0;
  }
  if !(strspn(s, msg)==0) return 1 lese return 0;
} 



void print_son()
{
  int i;
  char s[100];
  printf("\n hier ist der Sohn !!! \n");
  for (i=0;i<5;i++) 
  {
    sleep(3);
    printf("\n lese von der Pipe im %i. Lauf : ", i);
    switch (nread=read(pfd[0], s, sizeof(s)) )
    {
      case -1: printf(" READ-Error !!! \n"); break;
      case 0: printf(" EOF !!! \n"); break;
      default: printf(" %s ", s);break; 
    }
  }
  printf("\n Ende des Sohnes !! \n");
}

void print_father()
{
  int i;
  char s[100];
  printf("\n hier ist der Vater !!! \n");
  for (i=0;i<5;i++) 
  {
    sleep(1);
    sprintf(s, " hallo nr. %i ", i);
    printf("\n schreibe %s in die Pipe \n", s);
    if ( write(pfd[1], s, sizeof(s)) == -1 )
                       printf("\n write Error \n "); 
  }
}

int main()
{
  int pid;
  printf("\n ***** Start ---> !!!!! \n");

  if (pipe(pfd) == -1) 
  {
    printf("\n Fehler beim oeffnen der Pipe !!! \n");
    return -2;
  }


  pid=fork();
  switch (pid) 
  {
    case -1: printf("\n Fehler bei fork() \n");
             return -1;
    case 0:  print_son();
             return 0;
    default: print_father();
  }
  printf("\n Hier nochmal im Vater-Programm !!! \n");
  return 1;
} 
