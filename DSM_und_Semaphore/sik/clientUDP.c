#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <thread.h>
#include <stdio.h>
#include "UDP.h"

#define Pakete 10
#define BSIZE 82
 
sema_t empty,full;

struct {
        char data[BSIZE];
        int size;
        } buf;


   int sock, lenClient, count ;
   struct sockaddr_in client;
   struct hostent *hp;
   char machine[20] ;
   char msg[30];
   char DataStr[20];


void * producer(void * arg)
{
        while(1) {
                /* printf(" in producer vor ! "); */
                sema_wait(&empty);
                /*  printf(" .. nach  ! \n "); */
        
        	gets(msg);
        
        
                /*  printf( "p  msg :  %s \n", msg); */ 
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
                /*  printf(" .. nach  ! \n "); */ 
                if (buf.size == -1) break;
                /* printf( "rec msg   %s \n", msg); */
                printf( "c send %s\n", buf.data);
  
                strcpy(DataStr, buf.data);
                /* printf(" DataStr: %s \n",DataStr);  */
	        if (sendto (sock, DataStr , sizeof DataStr , 0,
                        (struct sockaddr *) &client, sizeof client) == -1)
      	        	perror(" sending DATAGRAM socket name !!!");

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

  thr_create(NULL,0,producer,NULL,THR_NEW_LWP,&tproducer);
   thr_create(NULL,0,consumer,NULL,THR_NEW_LWP,&tconsumer);
   thr_join(tconsumer,NULL,NULL);
 


  close( sock );
  exit(0);
 }
 else
 {
   printf("USAGE: %s <hostname> \n", argv[0]);
 }

}
    
