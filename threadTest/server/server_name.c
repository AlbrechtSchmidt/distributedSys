#include <thread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include "nameServer.h"
 
#define TRUE 1
#define MaxCache 100

/* UDP - Socket   TIME  -  SERVER */

char NetName[MaxCache][60]; /* 100 Namen je 60 Zeichen */
char IP_adr[MaxCache][30]; /* zugehoerige 100 IP Adressen mit je 30 Zeichen */
char retString[60];
char msg[100];
int count; 
int sock, lenClient, lenServer;
struct sockaddr_in server, client;

int  binding(char * H_name, char * IP_a)
{
  if (count < MaxCache)
  {
    strcpy(NetName[count],H_name);
    strcpy(IP_adr[count], IP_a);
    count++;
    return 1; 
  }
  return 0;
}

void lookup(char * H_name)
{
  int i, found;
  struct hostent *hp;
  found = 0;
  strcpy(retString,"Not found");
 /* for (i=0;i<count;i++)
  */ 
  i=0;
  do
  {
    if (strcmp(NetName[i], H_name) == 0) 
    {
        strcpy(retString, IP_adr[i]);
        printf("\n  Hostname: %s local found !!! \n", H_name);
        found=1;
    }
    i++;
  }
  while ( (i<count) && (found == 0));
  if (found==0) 
  {
    printf("\n  Ask for %s with gethostbyname !!! \n", H_name);
    hp = gethostbyname(H_name);
    if (hp == (struct hostent *) 0)
                 {
                   printf("\n gethostbyname returned : Not found !!! \n");
                   strcpy(retString,"Host Not found");
                 }
    else 
    {
          strcpy(retString,inet_ntoa(hp->h_addr));
          binding(H_name,retString);
    }
  } 
}

int unbind(char * H_name )
{
  int i, alt_count;
  alt_count=count;
  for (i=0;i<count;i++)
  {
    if (strcmp(NetName[i], H_name) == 0) 
    {
      count--;
      strcpy(NetName[i], NetName[count]);
      strcpy(IP_adr[i], IP_adr[count]);
    }
  }
  if (alt_count == count) return 0; else return 1;

} 


void * bearbeiten(void * arg)
{
	char buf[6][60];
	int n, ret;      
        printf("\n msg : %s ", msg);
 	   n=sscanf(msg, "%s%s%s%s%s", buf[0], buf[1], buf[2], buf[3], buf[4]);
       
	n--;
 
        printf("\n n=%i \n", n);
        sleep(1);
        printf("\n buf[0]= %s", buf[0]);
        sleep(1);
	printf("\n buf[1]= %s", buf[1]);
        printf("\n buf[2]= %s", buf[2]);
        sleep(1);
        printf("\n buf[3]= %s", buf[3]);
        sleep(1);
        printf("\n buf[4]= %s", buf[4]);
 
 
        if (n > 3) strcpy(retString, "Too many Args or Command not known !!!");
        
        else
        {
        if ( (strncmp("Bind", buf[0],1)==0) )
        {
           if (n==3)
           {
             ret = binding(buf[1],buf[2]);
             if (ret=1) strcpy(retString, "Binding ok !!!");
                else strcpy(retString, "out of Memory -- Binding fail !!!");
           }
           else strcpy(retString, "Bind with wrong Args !!!");
        } 
        else
        { 
           if (strncmp("Lookup", buf[0], 1)==0)
           {
             if (n==2)
             {
               /* printf("\n in main : %s \n", buf[1]); */
               lookup(buf[1]);
             }
             else strcpy(retString, "Lookup with wrong Args !!!");
           }
           else
           {
               if (strncmp("Unbind", buf[0], 1)==0)
               {
                  if (n==2)
                  {
                    ret = unbind(buf[1]);
                    if (ret==1) strcpy(retString, "unbinding ok !!!");
                      else strcpy(retString, "Not found -- unbinding fail !!!");
 
                  }
                  else strcpy(retString, "Unbind with wrong Args !!!");
               }
               else strcpy(retString,"Command not known !!!");
           }
        } 
       } 
        sendto(sock, retString, sizeof(retString), 0,
                   (struct sockaddr *) & client, sizeof(client));
        printf( "\n  ... send: %s \n", retString);
}

main()
{
   thread_t t;
 
   count = 0;
   
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
   
   do
   {
   	memset(msg, 0, sizeof msg);
   	recvfrom(sock, msg, sizeof(msg), 0, 
                  (struct sockaddr *) & client, &lenClient);
   	printf( "\nAnfrage > %s < empfangen \n", msg);
        if (strcmp(ENDE, msg)==0) break; 

        thr_create(NULL,0,bearbeiten,NULL,THR_NEW_LWP,&t);
     
   }
   while(TRUE);     
   /* Ende: UDP - Verbindung */
   

   printf( "\nKommunikation beenden !!!\n");
   close(sock);
   exit(0);
}
  

                
