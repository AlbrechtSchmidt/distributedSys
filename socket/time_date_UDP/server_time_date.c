#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <time.h>
#include "time_date.h"
 
#define TRUE 1
#define TimeStr "time"
#define DateStr "date"
#define DateTimeStr "all"
/* UDP - Socket   TIME  -  SERVER */



/* liest die Systemzeit und gibt die in einem 
   String zurueck                              */

char *Read_SysTime()
{
   time_t t;
   time(&t);
   return ctime(&t);
}



main()
{
   int sock, lenClient, lenServer ;
   struct sockaddr_in server, client;
   char buf[60];
   char timestring[60];
   
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

 
   printf( "Der Server laueft auf dem Socket port #%d\n", 
                     ntohs(server.sin_port));
   
   /*fuer UDP - Verbindung: */
         
   lenClient = sizeof(client);
   
   do
   {
   	memset(buf, 0, sizeof buf);
   	recvfrom(sock, buf, sizeof(buf), 0, 
                  (struct sockaddr *) & client, &lenClient);
   	printf( "\nAnfrage > %s < empfangen \n", buf); 
        if (strcmp(ENDE, buf)==0) break; 

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
   }
   while(TRUE);     
   /* Ende: UDP - Verbindung */
   

   printf( "\nKommunikation beenden !!!\n");
   close(sock);
   exit(0);
}
  

                
