#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>

#define DATA "hallo !!!"

main(int argc, char* argv[])
{
   int sock ;
   struct sockaddr_in name;
   struct hostent *hp;
 /*  struct hostent *gethostbyname();
  */ 
   sock = socket( AF_INET, SOCK_DGRAM, 0 );
   if (sock == -1) {
                     perror(" opening DATAGRAM socket !!!");
                     exit(1);
                   }
   
   hp = gethostbyname(argv[1]);
   if (hp == (struct hostent *) 0)
                 {
                   fprintf(stderr, "%s: unknown host \n", argv[1]);
                   exit(2);
                 }

  memcpy( (char *) &name.sin_addr, (char *) hp->h_addr, hp->h_length );  
  name.sin_family = AF_INET;
  name.sin_port = htons (atoi (argv[2]));

  if (sendto (sock, DATA, sizeof DATA, 0,
                 (struct sockaddr *) &name, sizeof name) == -1)
      perror(" sending DATAGRAM socket name !!!");
 
  close( sock );
  exit(0);
}
    
