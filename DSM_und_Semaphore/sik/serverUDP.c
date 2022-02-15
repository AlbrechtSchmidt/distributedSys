#include <sys/types.h>
#include <thread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include "UDP.h"
 
#define TRUE 1
#define BSIZE 82
 
sema_t empty,full;
 
struct {
        char data[BSIZE];
        int size;
        } buf;
 

/* UDP - Socket        SERVER */

int sock, lenClient, lenServer ;
struct sockaddr_in server, client;
char msg[30];

void * producer(void * arg)
{
        while(1) {
		/* printf(" in producer vor ! "); */
                sema_wait(&empty);
		/* printf(" .. nach  ! \n "); */
	
		memset(msg, 0, sizeof msg);
   		recvfrom(sock,msg, sizeof(msg), 0, 
                	  (struct sockaddr *) & client, &lenClient);
		
		/* printf( "p  msg :  %s \n", msg); */
                strcpy(buf.data, msg);
		buf.size=strlen(msg);

	        if (strcmp(ENDE, msg)==0) buf.size = -1; 
                sema_post(&full);
    		if (buf.size == -1 ) break;
  	 }
}


void * consumer(void * arg)
{
        while(1) {
		/* printf(" in consumer vor wait  ! "); */
                sema_wait(&full);
		/* printf(" .. nach  ! \n "); */
                if (buf.size == -1) break;
		/* printf( "rec msg   %s \n", msg); */
		printf( "%s\n", buf.data);
                sema_post(&empty);
                }
}
 





main()
{
   thread_t tproducer, tconsumer;
   sema_init(&empty,1,USYNC_THREAD,NULL);
   sema_init(&full,0,USYNC_THREAD,NULL);
       
   sock = socket( AF_INET, SOCK_DGRAM, 0 );
   if (sock == -1) {
                     perror(" opening DATAGRAM socket !!!");
                     exit(1);
                   }
   
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = htonl(INADDR_ANY);
   server.sin_port = htons(S_PORT);
   if (bind( sock, (struct sockaddr *) &server, sizeof server) == -1)
                  {
                    perror(" binding DATAGRAM socket !!!");
                    exit(1);
                  }
   
   
   lenServer = sizeof(server);
   if (getsockname (sock, (struct sockaddr *) &server, &lenServer) == -1)
                  {
                    perror(" getting DATAGRAM socket name !!!");
                    exit(1);
                  }

 
   printf( "Der Server laueft auf dem Socket port #%d\n", 
                     ntohs(server.sin_port));
   
   /*fuer UDP - Verbindung: */
         
   lenClient = sizeof(client);
   

   thr_create(NULL,0,producer,NULL,THR_NEW_LWP,&tproducer);
   thr_create(NULL,0,consumer,NULL,THR_NEW_LWP,&tconsumer);
   thr_join(tconsumer,NULL,NULL);

   
   /* Ende: UDP - Verbindung */
   

   printf( "\n Kommunikation beenden !!!\n");
   close(sock);
   exit(0);
}
  

                
