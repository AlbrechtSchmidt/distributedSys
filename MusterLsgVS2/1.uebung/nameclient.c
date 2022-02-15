/*************************************************************
 *                                                           *
 * Simple Nameserver --- Client Interface                    *
 * 1995 by Frank Kargl                                       *
 *                                                           *
 *************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "nameclient.h"

/*** Bind(Name,Adresse); traegt Namen in NS ein ***/
/*** Rueckgabewerte :  1-eingetragen ; 0-Fehler ***/

int Bind(char *name, char *ip) {
	int	sock;		/* Socket to Nameserver */
	char	dummystr[256];	/* Dummy String */
	extern	contact();
	extern	void discon();

	if ((sock=contact()) < 0) {
		perror("SNS: Can't contact nameserver");
		return(0);
	}

	sprintf(dummystr,"B %s %s\n",name,ip);
	
	write(sock,dummystr,strlen(dummystr));

	read(sock,dummystr,sizeof(dummystr));
	
	discon(sock);

	if (dummystr[0]=='O') return 1;
	else return 0;
}

/*** Unbind(Name); loescht Namen aus NS-Cache ***/
/*** Rueckgabewerte :  1-geloescht ; 0-Fehler ***/

int Unbind(char *name){
	int	sock;		/* Socket to Nameserver */
	char	dummystr[256];	/* Dummy String */
	extern	contact();
	extern	void discon();

	if ((sock=contact()) < 0) {
		perror("SNS: Can't contact nameserver");
		return(0);
	}

	sprintf(dummystr,"U %s\n",name);
	
	write(sock,dummystr,strlen(dummystr));

	read(sock,dummystr,sizeof(dummystr));
	
	discon(sock);

	if (dummystr[0]=='O') return 1;
	else return 0;
}

/*** Lookup(Name); liefert IP Adresse zurueck ***/

char *Lookup(char *name){
	int	sock;		/* Socket to Nameserver */
	static char	dummystr[256];	/* Dummy String */
	extern	contact();
	extern	void discon();

	if ((sock=contact()) < 0) {
		perror("SNS: Can't contact nameserver");
		return(0);
	}

	sprintf(dummystr,"L %s\n",name);
	write(sock,dummystr,strlen(dummystr));
	read(sock,dummystr,sizeof(dummystr));
	discon(sock);

	if (dummystr[0]=='E') return NULL;
	else return dummystr;
}

/*** Connect to server ***/

int contact() {
	int	sock;			/* socket to use */
	struct	sockaddr_in sock_addr;	/* Socket Address Structure */
	struct	hostent	*he;		/* Hostentry */
	char	*ptr;
	int	i;

	if ((sock=socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("Can't create socket");
		return -1;
	}

	if ( !isdigit(NAMESERVER[0]) ) {
		if ((he=gethostbyname(NAMESERVER)) == NULL) {
			perror("Can't gethostbyname");
			return -1;
		}
		memcpy(&(sock_addr.sin_addr.s_addr),(he->h_addr_list[0]),sizeof(long));
	} else {
		ptr=NAMESERVER;
		sock_addr.sin_addr.s_addr=inet_addr(ptr);
	}

	sock_addr.sin_port=htons(NAMEPORT);
	sock_addr.sin_family=AF_INET;

	if (connect(sock,&sock_addr,sizeof(sock_addr)) < 0) {
		perror("Can't connect");
		return -1;
	}

	return sock;
}

void discon(int sock) {
	shutdown(sock,2);
	close(sock);
}
