#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>
#include "time_date.h"


#define TimeStr "time"
#define DateStr "date"
#define DateTimeStr "all"
#define DELAYTIME 1 


/* liest die Systemzeit und gibt die in einem
   String zurueck                              */

char *Read_SysTime()
{
   time_t t;
   time(&t);
   return ctime(&t);
}



void ErrorHandler(strPtr) char* strPtr;{printf("Error: %s\n",strPtr); exit(-1);}

void UserSignalHandler(sig) int sig; { 
     signal(sig,UserSignalHandler);
     printf("PID = %d: Signal %d im SignalHandler angekommen\n",getpid(),sig);
     sleep(2); /* only for debugging */
}

main() {
   int  childpid, parentpid, retcode;
   do {
	signal(SIGCLD,SIG_IGN);
	signal(SIGUSR1,UserSignalHandler);

	if ((childpid = fork()) < 0) ErrorHandler("can`t fork");

	if (childpid > 0) {  

            int sock, lenClient, lenServer ;
            struct sockaddr_in server, client;
            char buf[60];
            char timestring[60];
 
            /* printf("\n hier ist der Vater !!! \n"); */
 
             sock = socket( AF_INET, SOCK_DGRAM, 0 );
             if (sock == -1) {
                               perror(" opening DATAGRAM socket !!!");
                               exit(1);
                             }
 
             server.sin_family = AF_INET;
             server.sin_addr.s_addr = htonl(INADDR_ANY);
             server.sin_port = htons(S_PORT);
             if (bind( sock, (struct sockaddr *) &server, sizeof server) == -1)
                  {
                    perror(" binding DATAGRAM socket !!!");
                    exit(1);
                  }
 
 
             lenServer = sizeof(server);
             if (getsockname (sock, (struct sockaddr *) &server, &lenServer) == -1)
                            {
                              perror(" getting DATAGRAM socket name !!!");
                              exit(1);
                            }
 
 
             printf( "\n\nDer Server laueft auf dem Socket port #%d\n",
                     ntohs(server.sin_port));
 
             /*fuer UDP - Verbindung: */
 
             lenClient = sizeof(client);
 
 
	            /* parent-process */
	     do {
	    
       	          memset(buf, 0, sizeof buf);
                  recvfrom(sock, buf, sizeof(buf), 0,
                            (struct sockaddr *) & client, &lenClient);
                  printf( "\nAnfrage > %s < empfangen \n", buf);
 
                  if (strcmp(ENDE, buf)==0)
                  {
                    printf("\n\nProgramm terminiert !!!\n\n");
                    kill(childpid,9);
                    exit(0);
                  }
 
 
                  if (strncmp(TimeStr, buf,1)==0)
                  {
                     strcpy(timestring, "Time : ");
                     strncat(timestring, (Read_SysTime()+11), 8);
                  }
                  else
                  {
                     if (strncmp(DateStr, buf, 1)==0)
                     {
                       strcpy(timestring, "Date : ");
                       strncat(timestring, Read_SysTime(), 11);
                       strncat(timestring, (Read_SysTime()+20), 4);
 
                     }
                     else
                     {
                         if (strncmp(DateTimeStr, buf, 1)==0)
                         {
                            strcpy(timestring, "Date and Time : ");
                            strncat(timestring, Read_SysTime(),24);
                         }
                         else strcpy(timestring,"Command not known !!!");
                     }
                  }
                  sendto(sock, timestring, sizeof(timestring), 0,
                             (struct sockaddr *) & client, sizeof(client));
                  printf( "... und %s geschickt\n", timestring);
 


                  retcode = kill(childpid,0 /* SIGUSR1 */);
	          /* printf("Vater pid = %d erhaelt zurueck: %d\n",
                                     getpid(),retcode);
                   */
                  if (retcode < 0 ) {
                  printf("\n -->> den Sohn neu aufstarten\n");
	          break;
	    }
	    sleep(DELAYTIME);
	  } while (1);
          close(sock);
	} else { 
	  /* child-process */
	  parentpid = getppid();
	  do {
	    retcode = kill(parentpid,0 /* SIGUSR1 */);
	    /* printf("Sohn  pid = %d erhaelt zurueck: %d\n",
                               getpid(),retcode);
	    */
            if (retcode < 0 ) {
	       printf(" -->> den Vater neu aufstarten\n");
	       break;
	    }
	    sleep(DELAYTIME);
	  } while(1);
        }
   } while (1);
}
