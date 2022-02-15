#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <thread.h>
#include <stdio.h>
#include "UDP.h"

#define Pakete 10
#define BSIZE 82
#define PAGESIZE 4096

int fd;
caddr_t result;

 
sema_t empty,full;

struct {
        int size;
        } buf;


int sock, lenClient, count ;
struct sockaddr_in client;
struct hostent *hp;
char machine[20] ;
char msg[30];
char * p;
char c;


/* 	Dies Routine wird bei einem SEGMENTATION-Signal angesprungen */
void segv(int a)
{
        caddr_t x;

	printf("SIGSEGV signal CODE=%i\n", a);
	
        /* Speicher auf READ/WRITE setzen ! */
	x = mmap((void *)result, PAGESIZE, 
		PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, fd, 0);

        /* Die "Seite" kann jetzt gesendet werden -- Semaphor erhoehen ! */
        sema_post(&full);
}


/* Hier wird ein String von der Tastatur eingelesen */
void * producer(void * arg)
{
        while(1) {
                /* Warten bis das alte Zeichen ausgegeben wurde */	        
                sema_wait(&empty);
        
                /* Zeichen einlesen */	        
                printf("p read >> "); 
        	gets(msg);
        
                strcpy(p, msg);
                buf.size=strlen(msg);
                
		if (strcmp(ENDE, msg)==0) buf.size = -1;
                if (buf.size == -1 ) break;
         }
}
 
/* Hier wird ein String abgeschickt */
void * consumer(void * arg)
{
	char DataStr[20];
        caddr_t x;
        while(1) {
                /* Warten bis ein Zeichen R/W vorhanden ist */	        
                sema_wait(&full);
                if (buf.size == -1) break;
  
                /* Zeichen absenden */	        
		strcpy(DataStr, p);
	        if (sendto (sock, DataStr , sizeof DataStr , 0,
                        (struct sockaddr *) &client, sizeof client) == -1)
      	        	perror(" sending DATAGRAM socket name !!!");

                printf("c send ! \n");

 
        	/* Speicher auf READ-only setzen ! */
		x = mmap((void *)result, PAGESIZE, 
			PROT_READ, MAP_SHARED|MAP_FIXED, fd, 0);
        	
		/* Signal-Handler setzen ! */
   		signal(SIGSEGV,segv);

        	/* Die "Seite" wurde gesendet -- Semaphor erhoehen ! */
                sema_post(&empty);
                }
}
 


/* UDP - Socket       CLIENT   */


int main(int argc, char * argv[])
{


 if (argc == 2)
 {
   thread_t tproducer, tconsumer;
   sema_init(&empty,1,USYNC_THREAD,NULL);
   sema_init(&full,0,USYNC_THREAD,NULL);
 
   /* Speicherseite anlegen */
   if ((fd = open("/dev/zero", O_RDWR)) == -1) return 1;
   result = mmap((void *)0, PAGESIZE, PROT_READ, MAP_SHARED, fd, 0);
   /* printf("address = %08X\n",result); */

   p=(char *)result;
   /* Signal-Handler setzen ! */
   signal(SIGSEGV,segv);


   sock = socket( AF_INET, SOCK_DGRAM, 0 );  /* fuer UDP - Verbindung*/
   if (sock == -1) {
                     perror(" opening DATAGRAM socket !!!");
                     exit(1);
                   }

   /* gethostname(machine, 20); */

   hp = gethostbyname(argv[1]);
   
   if (hp == (struct hostent *) 0)
                 {
                   fprintf(stderr, "%s: unknown host \n", machine);
                   exit(2);
                 }
  client.sin_family = AF_INET;

  memcpy( (char *) &client.sin_addr, (char *) hp->h_addr, hp->h_length );  

  client.sin_port = htons (S_PORT);

  /* threats anlegen */
  thr_create(NULL,0,producer,NULL,THR_NEW_LWP,&tproducer);
  thr_create(NULL,0,consumer,NULL,THR_NEW_LWP,&tconsumer);
  
  /* warten bis der consumer beendet ist ! */
  thr_join(tconsumer,NULL,NULL);
 


  close( sock );
  exit(0);
 }
 else
 {
   printf("USAGE: %s <hostname> \n", argv[0]);
 }

}
    
