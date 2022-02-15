#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include "TCP.h"

#define TRUE 1

main()
{
  int sock, length;
  struct sockaddr_in server;
  int msgsock;
  char buf[30];
  int rval;
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if ( sock == -1 )
  {
     perror("opening stream socket");
     exit(1);
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr =htonl( INADDR_ANY ); 
  server.sin_port = htons(S_PORT);
  if ( bind( sock, (struct sockaddr *)&server, sizeof server ) == -1 )
  {
     perror("binding stream socket");
     exit(1);
  }
 
  length = sizeof server;
  if ( getsockname( sock, (struct sockaddr *)&server, &length ) == -1)
  {
     perror("getting socket name");
     exit(1);
  }
  printf("Server auf Socket port #%d\n", ntohs(server.sin_port));
  
  listen( sock, 5 );
  do
  {
     msgsock = accept( sock, (struct sockaddr *)0, (int *)0 );
     if (msgsock == -1)
       perror("accept");
     else
       do
       {
          memset(buf, 0, sizeof buf); 
          if ((rval = read(msgsock, buf, sizeof(buf))) == -1)
            perror("reading stream message");
          /* if (rval == 0)
               printf("Ending connection\n"); */
          else
            {
                if ( write(msgsock, buf, sizeof(buf)) == -1)
                    perror("writeing stream message"); 
                /* printf("-->%s\n", buf); */
            }
          if (strcmp(ENDE, buf)==0) break; 
        } while (rval != 0);
       close(msgsock);
       if (strcmp(ENDE, buf)==0) break; 
  } while(TRUE);
  
  printf( "Kommunikation beenden !!!\n");
  
  close(sock);
  exit(0);
}

  
