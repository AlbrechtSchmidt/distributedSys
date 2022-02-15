#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define S_PORT 7777
#define TRUE 1

/* UDP - Socket        SERVER */

main()
{
	
	int 		sd, i;			/*fuer UDP - Verbindung*/
	struct sockaddr_in client, server;	/*fuer UDP - Verbindung*/
	
	char            buf[20];

	sd = socket(AF_INET, SOCK_DGRAM, 0);	/*fuer UDP - Verbindung*/
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(S_PORT);
	bind(sd, (struct sockaddr *) & server, sizeof(server));
	
	/*fuer UDP - Verbindung: */
	
	bzero(buf, sizeof(buf));
	i = sizeof(client);
	recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *) & client, &i);
	
	buf[0] = 'Y';
	sendto(sd, buf, sizeof(buf), 0, (struct sockaddr *) & client,
	sizeof(client));
	
	/* Ende: UDP - Verbindung */
	
	close(sd);
}
