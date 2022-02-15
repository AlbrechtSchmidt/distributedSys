#include "vertsys.h"

main(argc,argv) int argc; char* argv[];
{
	int             sockfd;
	struct sockaddr_in server;
	struct hostent *hp,*gethostbyname();
	char            buf[30],name[20];
	
	if (argc <=1 ) {
           printf("   Fehler: Mindestens einen Parameter angeben!\n");
           printf("     z.B.: client Date oder client Time oder client DateAndTime\n");
           printf("     oder: client Date \"hostname\"\n\n");
           exit(-1);
        }
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	/* TCP-Verbindung */ 

	if (argc == 3) hp = gethostbyname(argv[2]);
	else {	gethostname(name,20);
		hp = gethostbyname(name);
	}
	bzero ((char*) &server, sizeof(server));
	server.sin_family = AF_INET;
	bcopy((char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
	server.sin_port = htonl(SERVER_PORT);

	if (connect(sockfd,(struct sockaddr *) &server, sizeof(server)) < 0)
		{printf("Error: Client can`t connect to server\n"); exit(-1);}
	
	write(sockfd, argv[1],strlen(argv[1])+1);
	read(sockfd, buf, sizeof(buf));
	printf("   Request: %s\n   Answer : %s\n",argv[1], buf);
	close(sockfd);
	exit(0);
}
