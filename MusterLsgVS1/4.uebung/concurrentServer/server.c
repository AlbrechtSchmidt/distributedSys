#include "vertsys.h"

void ErrorHandler(strPtr) char* strPtr; {printf("Error: %s",strPtr); exit(-1);}

main() {
	struct timeval  timeVal;
	int             childpid, sockfd, newsockfd, clientLen;
	char            buf[20], *bufPtr;
	struct sockaddr_in client, server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);   /* TCP */
	bzero((char*) &server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(SERVER_PORT);
	if( bind(sockfd,(struct sockaddr *) &server,sizeof(server)) < 0)
	    ErrorHandler("Server can't bind local address\n");
        listen(sockfd,5);
	clientLen = sizeof(client);

	do {
	   newsockfd = accept(sockfd,(struct sockaddr *) & client, &clientLen);
	   if (newsockfd < 0) ErrorHandler("Server can't accept!\n");
	   if ((childpid = fork()) < 0) {
	 	close(newsockfd);
		ErrorHandler("Server can`t fork\n");
	   }

	   if (childpid == 0) {  /* child-process */
		close(sockfd);
		read(newsockfd, buf, sizeof(buf));
		/* Hier Unterscheidung nach date, time etc. einbauen */
		gettimeofday(&timeVal);
		bufPtr = ctime(&timeVal.tv_sec);
		write(newsockfd, bufPtr, strlen(bufPtr) + 1);
		exit(0);
 	  }
	  /* parent */
	  close(newsockfd);
	} while (TRUE);
}
