#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define S_PORT 7777
#define DATA "Echo-Serving"

/* UDP - Socket       CLIENT   */
main()
{
	int             sd;
	struct sockaddr_in server;
	struct hostent *hp,*gethostbyname();
	char            buf[20],name[20];
	
	int		i; /*zusaetzlich fuer die UDP - Version*/

	sd = socket(AF_INET, SOCK_DGRAM, 0);	/* fuer UDP - Verbindung*/ 

	gethostname(name,20);
	hp = gethostbyname(name);
	server.sin_family = AF_INET;
	bcopy((char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
	server.sin_port = htonl(S_PORT);
	
	/* UDP - Verbindung:  */
	
	sendto(sd, DATA, sizeof(DATA), 0, (struct sockaddr *)&server,
	sizeof(server));
	
	i = sizeof(server);
	recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&server, &i);
	
	printf("Client bekommt zurueck: %s\n", buf);
	
	close(sd);
	
        /* Ende der UDP - Verbindung */
}
