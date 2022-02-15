#include "Project.h"
#define HELP_CMD "help"
 
void main( int argc, char *argv[])
{
  int sock,l, ready, fd, rd_cnt, wrt_cnt;
  struct sockaddr_in server;
  struct hostent *hp, *gethostbyname();
  char buf[1024], cmd[50], LocalName[200];
  /* TCP Verbindung zum Server aufbauen 
	argv[1] : Hostname 
	argv[2] : Port
  */
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
	printf("\n dir        ...   Directory anzeigen");
	printf("\n get #      ...   Datei mit Nummer # anfordern");
	printf("\n put #      ...   Datei mit Nummer # zurueckschreiben");
	printf("\n\n bye        ...   vom Server abmelden");
	printf("\n kill       ...   Server abschalten\n");
      }


      /* Eingabe "kill" --> KILL_MSG an Server schicken */
      if (strncmp(cmd, KILL_MSG, strlen(KILL_MSG)) == 0)
      {
	if ( write(sock, KILL_MSG, sizeof KILL_MSG) == -1 )
 	    perror("writing on stream socket");
	break;
      }

      /* Eingabe "bye" --> CLOSE_MSG an Server schicken */
      if (strncmp(cmd, CLOSE_MSG, strlen(CLOSE_MSG)) == 0)
      {
	if ( write(sock, CLOSE_MSG, sizeof CLOSE_MSG) == -1 )
 	    perror("writing on stream socket");
	break;
      }

      /* Eingabe "dir" --> DIR_MSG an Server schicken */
      if (strncmp(cmd, DIR_MSG, strlen(DIR_MSG)) == 0)
      {
	if ( write(sock, DIR_MSG, sizeof DIR_MSG) == -1 )
 	    perror("writing on stream socket");
	printf("\nDirectory \n");
	/* Verzeichniseintraege empfangen und anzeigen bis 
	   Server das Ende signalisiert */  
	do
  	{
    		if (read(sock, buf, sizeof buf) == -1 )
        	  perror("reading on stream socket");
    		if (strncmp(EOT, buf, strlen(EOT))== 0) ready =1; 
		else 
		{	
			ready=0;
    			printf("%s", buf);
		}
  	}
  	while(!ready); 
 	printf("\n");
      }

      /* Eingabe "get ?" --> GET_MSG an Server schicken */
      if (strncmp(cmd, GET_MSG, strlen(GET_MSG)) == 0)
      {
	/* lokalen Dateinamen fuer die zu empfangende Datei festlegen */
	printf(" lokaler Dateiname: ");
	gets(LocalName);

        /* Wenn das File schon existiert wird es geloescht */
        if (access(LocalName, 0) != -1)
	{
		printf(" remove old File !!! \n"); 
		remove(LocalName);
	}

	/* File anlegen und zu schreiben oeffnen */
        if (( fd = open(LocalName, O_WRONLY | O_CREAT, MODE)) == -1 )
                {
                        printf("Can't open %s !\n", LocalName);
                }
	else 
	{
           if ( write(sock, cmd , strlen(cmd)) == -1 )
               perror("writing on stream socket");
           printf(" downloading file  ");
	
	   /* File blockweise lesen und in die Datei schreiben */
           do
           {
      		memset(buf, 0, sizeof buf); 
                if (read(sock, buf, sizeof buf) == -1 )
                  perror("reading on stream socket");
                if (strncmp(EOT, buf, strlen(EOT))== 0) ready =1;
                else
                {
                        ready=0;
			l=strlen(buf);
			if (l>1024) l=1024; /* maximale Blockgroesse darf 
				               nicht ueberschritten werden */
	              	if ( (wrt_cnt = write(fd, buf, l)) == -1)
			{
                            printf("Can't write in file %s !\n", LocalName);
                	}
		        printf(" ."); /* pro Block einen Punkt schreiben */
                }
            }
            while(!ready); /* Schleife beenden wenn EOT vom Server kommt */
	    printf("\n");
	}
      }


      /* Eingabe "put ?" --> PUT_MSG an Server schicken */
      if (strncmp(cmd, PUT_MSG, strlen(PUT_MSG)) == 0)
      {

	  /* lokalen Dateinamen fuer die zu empfangende Datei festlegen */
   	  printf(" lokaler Dateiname: ");
 	  gets(LocalName);

         /* File zu lesen oeffnen */
         if (( fd = open(LocalName, O_RDONLY)) == -1 )
         {
              /* im Fehlerfall Meldung ausgeben */
              printf("Can't open %s !\n", LocalName);
         }
         else
         {
	     /* Kommando an Server schicken */
             if ( write(sock, cmd , strlen(cmd)) == -1 )
               perror("writing on stream socket");
             printf(" transmitting file   ");
             /* sonst das File Blockweise uebertragen */
             memset(buf,0,(sizeof buf));
             while( (rd_cnt = read(fd, buf, sizeof buf)) > 0)
             {
                 /* printf("send %i bytes to client !\n", rd_cnt); */
		 printf(" .");
                 if ( (wrt_cnt=write(sock, buf, 1024)) == -1)
                           printf("socket write Error !\n");
                 memset(buf,0,(sizeof buf));
             }
             /* Ende signalisieren */
             write(sock, EOT, strlen(EOT));
          }
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
