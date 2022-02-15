#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
if (argc > 1)
   {
   struct hostent *hp;
   char machine[20];
   gethostname(machine, 20);
   hp = gethostbyname(argv[1]);
   
   if (hp == (struct hostent *) 0)
                 {
                   fprintf(stderr, "%s: unknown host \n", machine);
                   exit(2);
                 }
   
   printf("\n IP-Address of %s is %s \n",argv[1], inet_ntoa(hp->h_addr));
   }
else
   {
   printf("USAGE : hostadr <Hostname>\n");
   }
}
