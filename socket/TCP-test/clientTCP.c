#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include "TCP.h"

#define DATA "Echo-Serving"

/* TCP - Socket             CLIENT */


main()
{
  int sock, errcount, count;
  struct sockaddr_in server;
  struct hostent *hp, *gethostbyname();
  char buf[30], name[20];

  errcount = 0;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    perror("opening stream socket");
    exit(1);
  }

  gethostname(name,20);
  hp = gethostbyname(name);
        
  server.sin_family = AF_INET; 

  if ( hp == (struct hostent *)0 )
  {
    fprintf(stderr, "%s: unknown host\n", name);
    exit(2);
  }
  memcpy((char *) &server.sin_addr, (char *)hp->h_addr, hp->h_length);
  server.sin_port = htons(S_PORT);
  

  if (connect(sock, (struct sockaddr *)&server, sizeof server) == -1)
  {
    perror("connecting stream socket");
    exit(1);
  }
   for(count=0;count< MAX_COUNT ;count++)
  {
        /* printf("abgeschickt !!! \n"); */

 
       if ( write(sock, DATA, sizeof DATA) == -1 )
           perror("writing on stream socket");
  
       if ( read(sock, buf, sizeof(buf)) == -1 )
           perror("reading on stream socket");
        
       /* printf("Client bekommt zurueck: %s\n", buf); */

       if (strcmp(DATA, buf) != 0) 
       {
            printf(" Fehler ! (Durchlauf Nr. %i)\n", count );
            errcount++;
       }
        
       /* printf("Vergleich : %i \n", strcmp(DATA, buf)); */
  
  }      
  close(sock);
  printf("\ngesamte Fehlerzahl : %i\n", errcount);
  exit(0);
}

     
