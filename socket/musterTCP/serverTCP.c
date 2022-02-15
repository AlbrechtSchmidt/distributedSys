#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define S_PORT 7777
#define TRUE 1

/* TCP - Socket        SERVER */

main()
{
	int          sd, nsd; 		/*fuer TCP - Verbindung*/
	struct sockaddr_in server;     	/*fuer TCP - Verbindung*/

 	int count;
	
	char            buf[30], num[30];

	sd = socket(AF_INET, SOCK_STREAM, 0);  /*fuer TCP - Verbindung*/
	

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(S_PORT);
	bind(sd, (struct sockaddr *) & server, sizeof(server));
	
	/* fuer TCP - Verbindung: */
 	count = 1;
	
	listen(sd, 1);
	while (TRUE) {
		nsd = accept(sd, (struct sockaddr *) 0, (int *) 0);
		bzero(buf, sizeof(buf));
		read(nsd,buf,sizeof(buf));

                /*	sprintf(num, "%i. ", count);
			strcat(num, buf);	
 			printf("gelesen : %s in num : %s \n", buf, num); */

		write(nsd, buf, sizeof(buf));
		count++;
		close(nsd);
	}
	
	/* Ende: TCP - Verbindung */
	
	close(sd);
}
