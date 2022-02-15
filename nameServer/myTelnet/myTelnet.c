#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include "nameServer.h"
#include "favouriteHost.h"


/* UDP - Socket   */

int main()
{
   int sock, lenClient, count, errcount, n  ;
   struct sockaddr_in client;
   struct hostent *hp;
   char machine[20],s[3][40],cmdstr[40], buf[60], eof_cmd[5];
  
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
  strcpy(s[0],"Lookup");
  strcpy(s[1],myHost);

  for(n=0;n<2;n++)
  {
       if (sendto (sock, s[n], sizeof s[n]+10, 0,
           (struct sockaddr *) &client, sizeof client) == -1)
       perror(" sending DATAGRAM socket name !!!");
  }
  if (sendto (sock, eof_cmd, sizeof eof_cmd, 0,
      (struct sockaddr *) &client, sizeof client) == -1)
  perror(" sending DATAGRAM socket name !!!");
 


  lenClient =sizeof(client);

  if (recvfrom (sock, buf, sizeof(buf), 0, 
         (struct sockaddr *) &client, &lenClient ) == -1 )
     perror("receiving DATAGRAM socket name !!!"); 

  strcpy(cmdstr,"telnet ");
  strcat(cmdstr,buf);
  
  printf(" call:  %s\n",cmdstr );
                 
  close( sock );
  	
  system(cmdstr);
  exit(0);
}
    
