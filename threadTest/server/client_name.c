#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include "nameServer.h"


/* UDP - Socket    Name-Client */

int main(int argc, char * argv[])
{
if (argc > 1) 
   {
   int sock, lenClient, count, errcount, n  ;
   struct sockaddr_in client;
   struct hostent *hp;
   char msg[100], machine[20], buf[60], eof_cmd[5];
  
   strcpy(eof_cmd, END_OF_CMD);
    
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
  strcpy(msg, argv[1]);
  for(n=2;n<argc;n++)
  {
       strcat(msg, " ");
       strcat(msg, argv[n]);
  }
  strcat(msg, " ");
  strcat(msg, eof_cmd);
  printf("\n sende:>%s< \n", msg);
  if (sendto (sock, msg, sizeof msg, 0,
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
  printf("USAGE : client_name <REQ>\n");
  printf("        <REQ> ::= Bind <Name> <Address> | \n");
  printf("                  Lookup <Name>         | \n");
  printf("                  Unbind <Name> \n"); 
  }
}
    
