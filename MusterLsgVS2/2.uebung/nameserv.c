/*************************************************************
 *                                                           *
 * Simple Nameserver                                         *
 * 1995 by Frank Kargl                                       *
 *                                                           *
 *************************************************************/

#include <stdio.h>		/* stdio routines */
#include <search.h>		/* for btree routines */
#include <sys/types.h>		/* needed for sockets */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>		/* for DNS */
#include <synch.h>		/* for rw locks */
#include <thread.h>		/* for threads */

struct dbentry {
	char	name[128];	/* Hostname */
	char	netname[128];	/* Real Internet Name */
	char	ip[128];	/* ipaddress */
};

struct socketinfo {
	int	sock;		/* Socket Descriptor */
	struct	sockaddr_in sock_addr;	/* Socket Address Structure */
};

struct dbentry *root = NULL;	/* Root Ptr */
rwlock_t	mylock;		/* Used for locking database */
int	sock;			/* Socket used */
int	*rsock;			/* So printit() knows the socket */


void main(int argc, char **argv)
{

int	port;			/* port number to listen to */
struct	sockaddr_in sock_addr;	/* Socket Adress Structure */
int     rsize;
struct	socketinfo remsock;	/* Remote Socket Structure */
void *	handle_req();		/* Handle Requests */

	rwlock_init(&mylock, USYNC_THREAD, NULL);

	/*** Eval arguments ***/

	if ( argc!=2 ) {
		fprintf(stderr,"ERROR\nUsage: %s <portnum>\n",argv[0]);
		exit(1);
	} else {
		port = htons(atoi(argv[1]));
	}

	/*** Set-Up socket and listen ***/

	if ((sock=socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("Can't create socket");
		exit(1);
	}

	sock_addr.sin_port=port;
	sock_addr.sin_family=AF_INET;
	sock_addr.sin_addr.s_addr=INADDR_ANY;

	if (bind(sock,(struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
		perror("Can't bind socket");
		exit(1);
	}

	if(listen(sock,5) < 0) {
		perror("Can't listen to port");
		exit(1);
	}

	/*** Accept requests forever ***/

	while(1) {
		rsize=sizeof(remsock.sock_addr);
		if ((remsock.sock=accept(sock,(struct sockaddr *) &(remsock.sock_addr),&rsize)) < 0) {
			perror("Can't accept");
			continue;
		}

		if ( thr_create(NULL,0,&handle_req,(void *)&remsock,THR_DETACHED|THR_NEW_LWP,NULL) != 0 ) {
			perror("Can't create thread");
			continue;
		}
	}
}

/*** Handle Request ***/

void *handle_req(struct socketinfo *tempsock) {

int	remsock;		/* Remote Socket */
struct	sockaddr_in remsock_addr;	/* Remote Socket Address */
struct	hostent *he;		/* Remote hostentry */
char	hostname[128];		/* FQN of remote host */
struct	in_addr host_addr;	/* Still for nameresolving */
char	buffer[256];		/* Buffer for communication */
int	num;			/* Number of bytes read */
char	*ptr;			/* Dummy Ptr */
char	resp[128];		/* Response String */
struct	dbentry *tmpptr,*dummyptr;	/* temporary pointer in database */
char	**anyptr;
struct	dbentry dummydb;	/* needed for compare */
int	dbcompare();		/* routine for comparing 2 dbentries */
void	printit();		/* routine for printing dbentry */
char	arg1[80],arg2[80];	/* needed for sscanf's */


	remsock=tempsock->sock;
	remsock_addr=tempsock->sock_addr; 	/* ACHTUNG: eigentlich muesste hier kopiert werden ! */
	rsock=&remsock;

	if ((he=gethostbyaddr((char *) &(remsock_addr.sin_addr.s_addr),sizeof (remsock_addr.sin_addr.s_addr), AF_INET)) == NULL) {
		strcpy(hostname, inet_ntoa(remsock_addr.sin_addr));
	} else {
		strcpy(hostname,he->h_name);
	}

#ifdef DEBUG
	printf("Connection with %s\n",hostname);
#endif

	num=read(remsock, buffer, sizeof(buffer));
	buffer[num-1]='\0';
	if (buffer[num-2]=='\r') buffer[num-2]='\0';
	ptr=buffer;
	while(*ptr) { *ptr=tolower(*ptr); ptr++; }

#ifdef DEBUG
	printf("Request: %s\n",buffer);
#endif

	switch (*buffer) {

		/*** Bind 'b host (name|number) ***/

		case 'b':	if (sscanf(buffer,"b %s %s",arg1,arg2) != 2) {
					strcpy(resp,"ERROR\n");
					break;
				}
				
				tmpptr=(struct dbentry *)malloc(sizeof(struct dbentry));

				strcpy(tmpptr->name,arg1);

				if ( isdigit(*arg2) ) {
					host_addr.s_addr=inet_addr(arg2);
					if ((he=gethostbyaddr((char *) &(host_addr.s_addr),sizeof(host_addr.s_addr), AF_INET)) == NULL) {
						strcpy(resp,"ERROR\n");
						break;
					}
					strcpy(tmpptr->netname,he->h_name);
					strcpy(tmpptr->ip,arg2);
				} else {
					if ((he=gethostbyname(arg2)) == NULL) {
						strcpy(resp,"ERROR\n");
						break;
					}
					strcpy(tmpptr->netname,arg2);
					memcpy(&(host_addr.s_addr),(he->h_addr_list[0]),sizeof(long));
					strcpy(tmpptr->ip,inet_ntoa(host_addr));
				}

				if ( !rw_wrlock(&mylock)) {
					(void) tsearch((void *)tmpptr,(void **) &root, dbcompare);
					rw_unlock(&mylock);
				}

				strcpy(resp,"OK\n");
				break;

		/*** Lookup 'l host' ***/

		case 'l':	if (sscanf(buffer,"l %s",arg1) != 1 ) {
					strcpy(resp,"ERROR\n");
					break;
				}

				dummyptr=&dummydb;
				strcpy(dummyptr->name,arg1);

				if ( !rw_rdlock(&mylock)) {
					anyptr=tfind((void *)dummyptr,(void **) &root, dbcompare);
					rw_unlock(&mylock);
				}

				if (anyptr==NULL) {
					strcpy(resp,"NONAME\n");
					break;
				}

				tmpptr=(struct dbentry *)(*anyptr);

				strcpy(resp,tmpptr->ip);
				strcat(resp,"\n");
				break;

		/*** Unbind 'u host' ***/

		case 'u':	if (sscanf(buffer,"u %s",arg1) != 1) {
					strcpy(resp,"ERROR\n");
					break;
				}

				if ( !rw_wrlock(&mylock)) {
					anyptr=tdelete((void *)arg1,(void **) &root, dbcompare);
					rw_unlock(&mylock);
				}

				if (anyptr==NULL) {
					strcpy(resp,"ERROR\n");
					break;
				}

				strcpy(resp,"OK\n");
				break;

		/*** Dump 'd' ***/

		case 'd':	if ( !rw_rdlock(&mylock)) {
					twalk((void *) root, printit);
					rw_unlock(&mylock);
				}

				strcpy(resp,"OK\n");
				break;

		/*** Quit 'q' ***/

		case 'q':	printf("SNS: shutting down\n");
				shutdown(remsock,2);
				shutdown(sock,2);
				exit(0);

		default:	strcpy(resp,"ERROR\n"); break;

	}

#ifdef DEBUG
	printf("Response: #%s#\n",resp);
#endif

	write(remsock,resp,strlen(resp));

	shutdown(remsock,2);
	close(remsock);

}

/*** Database Compare ***/

int dbcompare(void *entry1, void *entry2) {
	return strcmp(((struct dbentry *)entry1)->name,((struct dbentry *)entry2)->name);
}

/*** Print DB Entries ***/

void printit(void **node, VISIT order, int level) {
	char *name,*netname,*ip;
	char buffer[256];

	if (order == postorder || order == leaf) {
		name = (*((struct dbentry **)node))->name;
		netname = (*((struct dbentry **)node))->netname;
		ip = (*((struct dbentry **)node))->ip;
		sprintf(buffer,"%s\t%s\t%s\n",name,netname,ip);
		write(*rsock,buffer,strlen(buffer));
	}
}
