#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

#define DATA "Was ist der Unterschied zwischen einem Krokodil?"

main( int argc, char *argv[])
{
  int sock;
  struct sockaddr_in server;
  struct hostent *hp, *gethostbyname();
  char buf[1024];

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    perror("opening stream socket");
    exit(1);
  }

  server.sin_family = AF_INET; 
  hp = gethostbyname(argv[1]);

  if ( hp == (struct hostent *)0 )
  {
    fprintf(stderr, "%s: unknown host\n", argv[1]);
    exit(2);
  }
  memcpy((char *) &server.sin_addr, (char *)hp->h_addr, hp->h_length);
  server.sin_port = htons(atoi(argv[2]));
  if (connect(sock, (struct sockaddr *)&server, sizeof server) == -1)
  {
    perror("connecting stream socket");
    exit(1);
  }
  
  if ( write(sock, DATA, sizeof DATA) == -1 )
    perror("writing on stream socket");
  close(sock);
  exit(0);
}

     
