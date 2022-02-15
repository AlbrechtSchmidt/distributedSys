#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include "UDP.h"
 
#define TRUE 1

/* UDP - Socket        SERVER */



main()
{
   int sock, lenClient, lenServer ;
   struct sockaddr_in server, client;
   char buf[30];
   
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
   	/* printf( "empfangen -->%s\n  ... und zurueck geschickt!\n", buf); */
        if (strcmp(ENDE, buf)==0) break; 
   	sendto(sock, buf, sizeof(buf), 0,
                   (struct sockaddr *) & client, sizeof(client)); 
   }
   while(1==1);     
   /* Ende: UDP - Verbindung */
   

   printf( "Kommunikation beenden !!!\n");
   close(sock);
   exit(0);
}
  

                
