/*  **********************************************************
    Uebung Verteilte Systeme SS95             Albrecht Schmidt      
  
    Programmieraufgabe Blatt 6: Compute - Server
                                
                                Client     

    Datum                                     Version 1
    ********************************************************** */

#include "Compute.h"
#define HELP_CMD "help"
 
void main( int argc, char *argv[])
{
  int key, sock,l, ready, fd, rd_cnt, wrt_cnt;
  struct sockaddr_in server;
  struct hostent *hp, *gethostbyname();
  char code[1024], buf[1024], cmd[50], LocalName[200];
  /* TCP Verbindung zum Server aufbauen 
	argv[1] : Hostname 
	argv[2] : Port
  */
  key = KEY;
  if (argc > 2)
  {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
      perror("opening stream socket");
      exit(1);
    }

    server.sin_family = AF_INET; 
    hp = gethostbyname(argv[1]);

    if ( hp == (struct hostent *)0 )
    {
      fprintf(stderr, "%s: unknown host\n", argv[1]);
      exit(2);
    }
    memcpy((char *) &server.sin_addr, (char *)hp->h_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (struct sockaddr *)&server, sizeof server) == -1)
    {
      perror("connecting stream socket");
      exit(1);
    }
  
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
	printf("\n\n bye                      ...   vom Server abmelden");
	printf("\n kill                     ...   Server abschalten\n");
      }


      /* Eingabe "kill" --> KILL_MSG an Server schicken */
      if (strncmp(cmd, KILL_MSG, strlen(KILL_MSG)) == 0)
      {
        encode(key, code,KILL_MSG );
	if ( write(sock, code, strlen(code) ) == -1 )
 	    perror("writing on stream socket");
	break;
      }

      /* Eingabe "bye" --> CLOSE_MSG an Server schicken */
      if (strncmp(cmd, CLOSE_MSG, strlen(CLOSE_MSG)) == 0)
      {
        encode(key, code,CLOSE_MSG );
	if ( write(sock, code, strlen(code) ) == -1 )
 	    perror("writing on stream socket");
	break;
      }

      /* Eingabe "getLoad" --> GET_MSG an Server schicken */
      if (strncmp(cmd, GET_MSG, strlen(GET_MSG)) == 0)
      {
        encode(key, code,GET_MSG );
	if ( write(sock, code, strlen(code) ) == -1 )
 	    perror("writing on stream socket");
        memset(code, 0, sizeof code); 
    	if (read(sock, code, sizeof code) == -1 )
       	  perror("reading on stream socket");
        decode(key, code, buf);
    	printf("Host: %s Load: %s\n", argv[1], buf);
      }

      /* Eingabe "remoteExec ?" --> EXEC_MSG an Server schicken */
      if (strncmp(cmd, EXEC_MSG, strlen(EXEC_MSG)) == 0)
      {
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
      }


    }
    while(TRUE); /* diese Schleife wird durch ein break 
		    bei einem "kill" oder "bye" Kommando verlassen */
    close(sock);
    printf("\n... bye !\n");
    exit(0);
  }
  else
  {
	printf("\nUSAGE: %s <hostname> <port>\n\n", argv[0]);
  }
}
