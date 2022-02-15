#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include "nameServer.h"

/* UDP - Socket       rmServer   */

main()
{
   int sock, lenClient ;
   struct sockaddr_in client;
   struct hostent *hp;
   char machine[20], buf[60];
   
   sock = socket( AF_INET, SOCK_DGRAM, 0 );  /* fuer UDP - Verbindung*/
   if (sock == -1) {
                     perror(" opening DATAGRAM socket !!!");
                     exit(1);
                   }
   gethostname(machine, 20);
   hp = gethostbyname(machine);
   
   if (hp == (struct hostent *) 0)
                 {
                   fprintf(stderr, "%s: unknown host \n", machine);
                   exit(2);
                 }
  client.sin_family = AF_INET;

  memcpy( (char *) &client.sin_addr, (char *) hp->h_addr, hp->h_length );  

  client.sin_port = htons (S_PORT);

  printf("Remove Signal abgeschickt !!! \n"); 

  if (sendto (sock, ENDE, sizeof ENDE, 0,
        (struct sockaddr *) &client, sizeof client) == -1)
        	perror(" sending DATAGRAM socket name !!!");
 
  close( sock );
  exit(0);
}
    
