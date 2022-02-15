#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

main()
{
   int sock, length;
   struct sockaddr_in name;
   char buf[1024];
   
   sock = socket( AF_INET, SOCK_DGRAM, 0 );
   if (sock == -1) {
                     perror(" opening DATAGRAM socket !!!");
                     exit(1);
                   }
   
   name.sin_family = AF_INET;
   name.sin_addr.s_addr = INADDR_ANY;
   name.sin_port = 0;
   if (bind( sock, (struct sockaddr *) &name, sizeof name) == -1)
                  {
                    perror(" binding DATAGRAM socket !!!");
                    exit(1);
                  }
   
   length = sizeof(name);
   if (getsockname (sock, (struct sockaddr *) &name, &length) == -1)
                  {
                    perror(" getting DATAGRAM socket name !!!");
                    exit(1);
                  }

   printf( "socket port #%d\n", ntohs(name.sin_port));
   if (read(sock, buf ,1024) == -1)
                    perror(" receiving DATAGRAM packet !!!");
   
   printf( "-->%s\n", buf);
   close(sock);
   exit(0);
}
  

                
