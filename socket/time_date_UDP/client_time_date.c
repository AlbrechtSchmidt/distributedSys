#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include "time_date.h"


/* UDP - Socket    TIME -  CLIENT   */

int main(int argc, char * argv[])
{
if (argc > 1) 
   {
   int sock, lenClient, count, errcount  ;
   struct sockaddr_in client;
   struct hostent *hp;
   char machine[20], buf[60];
   
   errcount = 0;
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

  

  /* printf("abgeschickt !!! \n"); */

  if (sendto (sock, argv[1], sizeof argv[1], 0,
         (struct sockaddr *) &client, sizeof client) == -1)
     perror(" sending DATAGRAM socket name !!!");
 
  lenClient =sizeof(client);

  if (recvfrom (sock, buf, sizeof(buf), 0, 
         (struct sockaddr *) &client, &lenClient ) == -1 )
     perror("receiving DATAGRAM socket name !!!"); 

  
  printf(" Server said:  %s\n",buf );
                 
  close( sock );
  exit(0);
  }
  else
  {
  printf("USAGE : client_time_date <REQ>\n");
  printf("        <REQ> ::= time | date | all\n"); 
  }
}
    
