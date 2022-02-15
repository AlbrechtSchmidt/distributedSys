/* ==================================================================== */
/*	einfacher DSM Server		S.Traub				*/
/* ====================================================================	*/

#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/* mogeln wegen UDP Paketgroesse */
#undef PAGESIZE
#define PAGESIZE 1024

/* Kommunikationsport fuer DSM Server */
#define REQUEST_PORT 3000
#define DATA_PORT (REQUEST_PORT+1)

thread_t comm;		/* Thread ID des Kommunikationsthreads 		*/
caddr_t	 myadr;		/* Adresse des gemeinsamen DSM-Bereichesi	*/
int	 fd;		/* Filedescriptor fuer /dev/zero		*/

int data_socket;		/* Socket zum Austausch der DSM Daten	*/
struct sockaddr_in rem_req;	/* Internetadresse der Gegenstation (Request)*/
struct sockaddr_in rem_data;	/* "	"	"	"	"   (Data) */


/* ====================================================================== */
/*   Hilfsfunktion zum Initialisieren eines Sockets			  */

void SetupSocket(int * s, int portnum)
{
	struct sockaddr_in sadr;

*s = socket(PF_INET,SOCK_DGRAM,0);
sadr.sin_family = AF_INET;
sadr.sin_addr.s_addr = htonl(INADDR_ANY);	
sadr.sin_port = htons(portnum);
bind(*s, (struct sockaddr *)&sadr, sizeof(sadr));
}

/* ====================================================================== */
/*  Kommunikations Thread					          */

void * CommThread(void * arg)
{
	int numb;
	int rcv;
	char Request;
	struct sockaddr_in client;

SetupSocket(&rcv,REQUEST_PORT);
for(;;) {
	/* Auf Request warten */
	recvfrom(rcv,
		&Request, 1, 0, 
		(struct sockaddr *)&client, &numb);

	/* Daten senden */
	sendto(		rcv, 
			(void *)myadr, PAGESIZE, 0, 
			(struct sockaddr *)&rem_data, sizeof(rem_data));

	/* DSM-Seite lokal sperren */
	mmap((void *)myadr, PAGESIZE, PROT_NONE, MAP_SHARED|MAP_FIXED, fd, 0);
	}
}


/* ====================================================================	*/
/*	Pagefault-Handler (reagiert auf SEGV Signal)			*/

void pagefault(int a)
{
	struct sockaddr_in client;
	char Request = 0;
	int numb;

/* Request an Server senden */
sendto(	data_socket,
	&Request,1,0,	/* nur 1 Byte senden, Inhalt ist unbedeutend */
	(struct sockaddr *)&rem_req, sizeof(rem_req));

/* Seite zugreifbar setzen */
mmap(	(void *)myadr, PAGESIZE, 
	PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, fd, 0);

/* auf Antwort warten */
recvfrom(	data_socket,
		(void *)myadr, PAGESIZE, 0, 
		(struct sockaddr *)&client, &numb);

signal(SIGSEGV,pagefault);
}


/* ====================================================================	*/
/*   DSM Initialisieren							*/

int dsm_init(char * remotename, int client)
{
	int pcode;
	struct hostent * he;

	/* Klient hat die DSM-Seite zuerst */
	if (client) pcode = PROT_READ|PROT_WRITE; else pcode = PROT_NONE;

	/* DSM-Seite anlegen auf beliebiger ADresse */
	if ((fd =open("/dev/zero", O_RDWR)) == -1) return 0;
	myadr = mmap((void *)0, PAGESIZE, pcode, MAP_SHARED, fd, 0);

	/* Internetadresse der Gegenstation bestimmen */
	he = gethostbyname(remotename);
	if (he==NULL) { printf("cannot resolve %s\n",remotename); exit(1); }

	/* Adressstrukturen initialisieren */
        rem_req.sin_family = AF_INET;
        memcpy(&rem_req.sin_addr, he->h_addr, he->h_length);
        rem_req.sin_port = htonl(REQUEST_PORT);
	memcpy(&rem_data,&rem_req,sizeof(rem_data));
	rem_data.sin_port = htonl(DATA_PORT);
	SetupSocket(&data_socket,DATA_PORT);

	/* Pagefaulthandler anmelden */
	signal(SIGSEGV,pagefault);

	/* Kommunikationsthread starten	*/
	thr_create(NULL,0,CommThread,NULL,THR_NEW_LWP,&comm);
	return 1;
}

/* bis hier ist der Code zur DSM-Verwaltung, dies koennte auch eine
   Bibliothek sein.
------------------------------------------------------------------------
   Anwendungscode beginnt hier 
*/

main(int argc, char * argv[])
{
	char line[80];
	int client;

	client = argc>=3;	/* Klient, falls irgend ein zusaetzliches
				   Argument angegeben wird */

	dsm_init(argv[1], client);

	if (client) {
		printf("client\n--------------------\n");
		do {
			gets(line);
			strcpy((char *) myadr, line);
			} while (line[0]);

	} else {

		printf("server\n--------------------\n");
		do {
			strcpy(line,(char *)myadr);
			printf("data: %s\n",line);
			sleep(3);
			} while(1);
		}

}

