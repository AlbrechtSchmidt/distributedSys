#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include "UDP.h"

#define DATA "Echo-Servering"


/* UDP - Socket       CLIENT   */


main(int argc, char* argv[])
{
   int sock, lenClient, count, errcount  ;
   struct sockaddr_in client;
   struct hostent *hp;
   char machine[20], buf[30];
   
   errcount = 0;
   sock = socket( AF_INET, SOCK_DGRAM, 0 );  /* fuer UDP - Verbindung*/
   if (sock == -1) {
                     perror(" opening DATAGRAM socket !!!");
                     exit(1);
                   }

   /* gethostname(machine, 20); */

   hp = gethostbyname(argv[1]);
   
   if (hp == (struct hostent *) 0)
                 {
                   fprintf(stderr, "%s: unknown host \n", machine);
                   exit(2);
                 }
  client.sin_family = AF_INET;

  memcpy( (char *) &client.sin_addr, (char *) hp->h_addr, hp->h_length );  

  client.sin_port = htons (S_PORT);

  for(count=0;count< MAX_COUNT ;count++)
  {
	/* printf("abgeschickt !!! \n"); */

	if (sendto (sock, DATA, sizeof DATA, 0,
             (struct sockaddr *) &client, sizeof client) == -1)
      		perror(" sending DATAGRAM socket name !!!");
 
	lenClient =sizeof(client);

  	if (recvfrom (sock, buf, sizeof(buf), 0, 
                 (struct sockaddr *) &client, &lenClient ) == -1 )
	perror("receiving DATAGRAM socket name !!!"); 

  	
        /*  printf("Client bekommt zurueck: %s\n", buf); */
  
        if (strcmp(DATA, buf) != 0) 
        {
            printf(" Fehler ! (Durchlauf Nr. %i)\n", count );
            errcount++;
        }
        
 	/* printf("Vergleich : %i \n", strcmp(DATA, buf)); */
  
  }
  close( sock );
  printf("\ngesamte Fehlerzahl : %i\n", errcount);
  exit(0);
}
    
