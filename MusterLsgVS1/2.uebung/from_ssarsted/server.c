#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/file.h>

#define DEFAULT_TIMEOUT 5

FILE   *file;
int    pipe_fd[2];
char   buf[20];
struct stat puf;
struct timeval tp_b, tp_e, tp;
char   time_server_ok, control_server_ok;
int    timeout;

main(int argc, char *argv[])
{
   if (argc > 1)
     timeout = atoi(argv[1]);
   else
     timeout = DEFAULT_TIMEOUT;

   printf("TIMESERV v0.5\n");
   printf("=============\n");
   printf("Timeout: %d\n\n", timeout);
  
   if ((file = fopen("medium", "r+")) == NULL)
   {
      file = fopen("medium", "w+");
      fputc('F', file);
      fflush(file);
   }

 
   if (pipe(pipe_fd) == -1 )
   {
     perror("creating pipe");
     exit(1);
   }

   while(1) 
   {
     switch(fork())
     {
       case 0:  /* child */
                printf("control server invoked\n");
                time_server_ok = 1;
                while(time_server_ok)
                {
                   printf("control server: sending keep-alive message\n");
                   write(pipe_fd[0], "*", 1); 

                   printf("control server: waiting for keep-alive msg. from time server\n");
                   *buf = ' ';
                   time_server_ok = 0;
 		   gettimeofday(&tp_b);
                   do
                   { 
                     /* hier Abfrage, ob Pipe leer ist */
                     fstat(pipe_fd[0], &puf);
                     if(puf.st_size != 0)
                     {
                       /* wenn Pipe nicht leer, dann lesen */
                       read(pipe_fd[0], buf, 1);
                       time_server_ok = 1;
                     }
                     gettimeofday(&tp_e);
                   } while (((tp_e.tv_sec - tp_b.tv_sec) <= timeout) && !time_server_ok);
                   
                   if (time_server_ok)
                     printf("control server: response received '%c'\n", *buf);
                   else
                     printf("control server: time server down, sorry\n");
                   sleep(1);
                }
                break;
       default: /* parent */
                printf("time server invoked\n");
                control_server_ok = 1;
                while(control_server_ok)
                {
                   flockfile(file);
                   rewind(file);
                   gettimeofday(&tp);
     		   fprintf(file, "%15ld", tp.tv_sec);
		   fflush(file);
		   funlockfile(file); 

                   printf("time server: written %5d\n", tp.tv_sec);
                   printf("time server: sending keep-alive message\n");
                   write(pipe_fd[1], "*", 1); 

                   printf("time server: waiting for keep-alive msg. from control server\n");
                   *buf = ' ';
                   control_server_ok = 0;
 		   gettimeofday(&tp_b);
                   do
                   {
                     fstat(pipe_fd[1], &puf);
                     if(puf.st_size != 0)
                     {
                       read(pipe_fd[1], buf, 1);
                       control_server_ok = 1;
                     }
                     gettimeofday(&tp_e);
                   } while (((tp_e.tv_sec - tp_b.tv_sec) <= timeout) && !control_server_ok);
                   
                   if (control_server_ok)
                     printf("time server: response received '%c'\n", *buf);
                   else
                     printf("time server: control server down, sorry\n");
                   sleep(1);
	        }
                break;
      }
   }
}

