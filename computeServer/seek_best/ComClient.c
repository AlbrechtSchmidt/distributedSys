/*  **********************************************************
    Uebung Verteilte Systeme SS95             Albrecht Schmidt      
  
    Programmieraufgabe Blatt 6: Compute - Server
                                
                                Client     

    Datum 12.7.95                                    Version 1
    ********************************************************** */

#include "Compute.h"
#define HELP_CMD "help"
#define MAXLOAD 1000 
  

int key = KEY;
int maxhosts;
char computeHost[5][100];

/* Diese Funktion baut eine Socket TCP-Verbindung auf */
/* der Rueckgabewert ist der Socketdescriptor         */
int InitTCP(char * host, int s_port)
{
  int sockId;
  struct hostent *hp, *gethostbyname();
  struct sockaddr_in server;
  sockId = socket(AF_INET, SOCK_STREAM, 0);
  if ( sockId == -1 )
  {
     perror("opening stream socket");
     exit(1);
  }
  server.sin_family = AF_INET;
  hp = gethostbyname(host);

  if ( hp == (struct hostent *)0 )
  {
    fprintf(stderr, "%s: unknown host\n", host);
    exit(2);
  }

  memcpy((char *) &server.sin_addr, (char *)hp->h_addr, hp->h_length);

  server.sin_port = htons(s_port);  /* den Port festlegen */
  if (connect(sockId, (struct sockaddr *)&server, sizeof server) == -1)
  {
     perror("connecting stream socket");
     exit(1);
  }
  
  /* printf("TCPSocket installed on Port #%d\n", ntohs( server.sin_port)); */
  return sockId;
}
 
void sendKill(int sock)
{
   char code[1024];
   encode(key, code,KILL_MSG );
   if ( write(sock, code, strlen(code) ) == -1 )
 	    perror("writing on stream socket");
}


void sendBye(int sock)
{
   char code[1024];
   encode(key, code,CLOSE_MSG );
   if ( write(sock, code, strlen(code) ) == -1 )
     perror("writing on stream socket");
   exit(1);
}

float sendGetLoad(int sock)
{
        char buf[1024], code[1024];
        float load;
        encode(key, code,GET_MSG );
        if ( write(sock, code, strlen(code) ) == -1 )
            perror("writing on stream socket");
        memset(code, 0, sizeof code);
        if (read(sock, code, sizeof code) == -1 )
          perror("reading on stream socket");
        decode(key, code, buf);
        sscanf(buf, "%f", &load);
        /* printf("str >%s< Load: %f\n",buf,  load); */
        return load;
}
 
float sendRemoteExec(int hostNr, char * cmd)
{
        char buf[1024], code[1024];
        int sock, ready;
        sock = InitTCP(computeHost[hostNr], S_PORT);
        encode(key, code,cmd );
        if ( write(sock, code, strlen(code) ) == -1 )
            perror("writing on stream socket");
 
        /* File blockweise lesen und ausgeben */
        do
        {
             memset(code, 0, sizeof code);
             memset(buf, 0, sizeof buf);
             if (read(sock, code, sizeof code) == -1 )
               perror("reading on stream socket");
             decode(key, code, buf);
             if (strncmp(EOT, buf, strlen(EOT))== 0) ready =1;
             else
             {
                     ready=0;
                     printf("%s",buf);
             }
         }
         while(!ready); /* Schleife beenden wenn EOT vom Server kommt */
         printf("\n");
         close(sock);
}

int InitHosts()
{
  strcpy(computeHost[0], "aurelian");
  strcpy(computeHost[1], "catull");
  strcpy(computeHost[2], "commodus");
  strcpy(computeHost[3], "plautus");
  strcpy(computeHost[4], "aemnius");
  return 5;
}

void killAll()
{
   int i, sock;
   for(i=0;i<maxhosts;i++)
   {
      sock = InitTCP(computeHost[i], S_PORT);
      sendKill(sock);
      close(sock);
   }
}
 

int GetBestHost()
{
   int i, sock, bestHost;
   float load, bestLoad;
   bestHost=0;
   bestLoad=MAXLOAD;
  
   for(i=0;i<maxhosts;i++)
   {
      sock = InitTCP(computeHost[i], S_PORT);
      load = sendGetLoad(sock);
      /* printf("Host: %s , Load: %f , best %f \n", computeHost[i], load); */
      if (load <= bestLoad) 
      {
         /* printf("wechsel \n"); */
         bestHost=i;
         bestLoad = load;
      }
    }
    close(sock);
    return bestHost;
}

 
void main( int argc, char *argv[])
{
  int  hostNr;
  char cmd[50];
  maxhosts = InitHosts();

 
    /* Eingabeschleife mit ">" als Userprompt */
    do
    {
      printf("\n>");
      gets(cmd);
      /* Kommando einlesen */

      
      /* Eingabe "help" */
      if (strncmp(cmd, HELP_CMD, strlen(HELP_CMD)) == 0)
      {
	printf("\n Kurzhilfe:");
	printf("\n getLoad                  ...   getLoad auf dem Rechner");
	printf("\n remoteExec <befehl>      ...   RemoteExec des Befehls");
      }


      /* Eingabe "kill" */
      if (strncmp(cmd, KILL_MSG, strlen(KILL_MSG)) == 0)
      {
            killAll();
            break;
      }


      /* Eingabe "bye" */
      if (strncmp(cmd, CLOSE_MSG, strlen(CLOSE_MSG)) == 0)
            break;
 
      /* Eingabe "getLoad" --> GET_MSG an Server schicken */
      if (strncmp(cmd, GET_MSG, strlen(GET_MSG)) == 0)
      {
           hostNr = GetBestHost();
           printf("best Host %s\n", computeHost[hostNr]); 
      }

      /* Eingabe "remoteExec ?" --> EXEC_MSG an Server schicken */
      if (strncmp(cmd, EXEC_MSG, strlen(EXEC_MSG)) == 0)
      {
           hostNr = GetBestHost();
           printf("best Host: %s\n\n", computeHost[hostNr]); 
           sendRemoteExec(hostNr, cmd);
      }

    }
    while(TRUE); /* diese Schleife wird durch ein break 
		    bei einem "kill" oder "bye" Kommando verlassen */
    printf("\n... bye !\n");
    exit(0);
}
