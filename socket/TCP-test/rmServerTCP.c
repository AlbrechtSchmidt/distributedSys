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
  int sock;
  struct sockaddr_in server;
  struct hostent *hp, *gethostbyname();
  char buf[30], name[20];

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
 
  printf("sende: Server STOP \n");
 
  if ( write(sock, ENDE, sizeof ENDE) == -1 )
    perror("writing on stream socket");
        
  close(sock);
  exit(0);
}

     
