#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define S_PORT 7777
#define DATA "Echo-Serving"

/* TCP - Socket             CLIENT */

main()
{
	int             sd;
	int i, comp_res;
	struct sockaddr_in server;
	struct hostent *hp,*gethostbyname();
	char            buf[30],name[20];
	

	sd = socket(AF_INET, SOCK_STREAM, 0);   /* fuer TCP - Verbindung */ 

	gethostname(name,20);
	hp = gethostbyname(name);
	server.sin_family = AF_INET;
	bcopy((char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
	server.sin_port = htonl(S_PORT);
	
	/* TCP - Verbindung aufbauen: */ 
	
	bind(sd, (struct sockaddr *) & server, sizeof(server));
	
        for(i=0;i<1000;i++)
	{
		connect(sd, (struct sockaddr *) & server, sizeof(server));
	
		write(sd, DATA, sizeof(DATA));
		read(sd, buf, sizeof(buf));
	 	comp_res = strcmp(DATA, buf);
		/* printf("Transfer Nr.: %i  Ergebnis: %i", i, comp_res);
		   printf("Client bekommt zurueck: %s\n", buf); */
		if (comp_res != 0) printf("Fehler beim %i ten Transfer\n",i);
		close(sd);
	}

	/* Ende der TCP - Verbindung */
}
