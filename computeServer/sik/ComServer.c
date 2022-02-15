/*  ********************************************************** 
    Uebung Verteilte Systeme SS95             Albrecht Schmidt
    
    Programmieraufgabe Blatt 6: Compute - Server
                                  

    Der PFS-Server ist Thread-basierend, er laesst sich also
    nur auf Solaris (neuer Pool) compilieren.
 
    Datum                                     Version 1 
    ********************************************************** */ 
 

#include "Compute.h" 
#include <signal.h>     /* kill() wird benoetigt um aus dem Thread das 
                           Programm zu beenden */  
#include <thread.h>     /* fuer die Threads */
#include <dirent.h>     /* zum lesen des File-Directory */
 
#define HELP_ARG  "-help"
 

typedef struct	        /* Parameter fuer die Threadfunktion in struct */
{	                /* speichern .., falls noch weiter Parameter dazu kommen */
	int msock;
} ThreadArg;

typedef ThreadArg * PThreadArg; 




int showHelp(char * Pname)
{
  printf("\nUSAGE: %s [<port>]\n\n", Pname);
  printf("Wird <port> nicht angegeben waehlt das System einen aus !\n");
  exit(1);
}


/* Diese Funktion baut eine Socket TCP-Verbindung auf */
/* der Rueckgabewert ist der Socketdescriptor         */
int InitTCP(struct sockaddr_in * server, int s_port)
{
  int sockId, length;
  sockId = socket(AF_INET, SOCK_STREAM, 0);
  if ( sockId == -1 )
  {
     perror("opening stream socket");
     exit(1);
  }
  (*server).sin_family = AF_INET;
  (*server).sin_addr.s_addr = INADDR_ANY;
  (*server).sin_port = htons(s_port);  /* den Port festlegen */
  if ( bind( sockId, (struct sockaddr *) server, sizeof (*server) ) == -1 )
  {
     perror("binding stream socket");
     exit(1);
  }
  length = sizeof server;
  if ( getsockname( sockId, (struct sockaddr *)server, &length ) == -1)
  {
     perror("getting socket name");
     exit(1);
  }
  printf("Socket installed on Port #%d\n", ntohs( (*server).sin_port));
  return sockId;
}



/* Die thread-Funktion, sie bearbeitet die Anfragen */
void * thread_func(void * arg)
{
  PThreadArg p = (PThreadArg)arg; /* Parameter in Variablem umwandeln */
  char buf[1024], befehl[30], para[100], syscall[100]; 
  int rval, para_num, msgsock;
  int fd, rd_cnt, wrt_cnt;
  msgsock = p->msock; /* locale Variable mit Parameter belegen */
  do 
  { 
      memset(buf, 0, sizeof buf);    
      /* Anfrage lesen vom Socket */
      if ((rval = read(msgsock, buf, 1024)) == -1)
            perror("reading stream message");
    
      /* printf("%s", buf); */
 
      /* Der Client schickt ein "bye" --> Verbindung abbauen */
      if (strncmp(CLOSE_MSG, buf, strlen(CLOSE_MSG))==0)
      { 
                break;
      } 

      /* Der Client schickt ein "kill" --> Verbindung abbauen 
					   und Server anhalten */
      if (strncmp(KILL_MSG, buf, strlen(KILL_MSG))==0)
      { 
		 close(msgsock);
		 printf("\nreceived KILL !\n");
		 kill(getpid(), SIGTERM);
      }    

      /* Message in Befehl und Parameter zerlegen */
      para_num=sscanf(buf, "%s%s", befehl, para);
         
      strcpy(para, buf+strlen(befehl)+1);
         
      if (para_num < 1) /* leere Befehl */
      { 
             strcpy(befehl, "");
             strcpy(para,"");
      } 
      if (para_num == 1) /* Befehl ohne Parameter */
      { 
             strcpy(para,"");
      } 

      /* printf("-->Befehl: %s %s \n", befehl, para); */
            
      /* Der Client schickt "getLoad" --> Lastangabe schicken */
      if ( strncmp (GET_MSG, befehl, strlen(GET_MSG))==0)
      {
        printf("GetLoad\n");
        system("uptime | awk -F\" \" '{print $10}' | tr \",\" \" \" > load.tmp ");

        /* File zu lesen oeffnen */
	if (( fd = open("load.tmp", O_RDONLY)) == -1 )
        {
		/* im Fehlerfall Meldung schicken */
		memset(buf,0,(sizeof buf));
		sprintf(buf, "Can't open load.tmp !\n");
		printf("%s", buf);
		write(msgsock, buf, strlen(buf));
         }
         else
         {
		memset(buf,0,(sizeof buf));
	        if( (rd_cnt = read(fd, buf, sizeof buf)) > 0)
		{
		    if ( (wrt_cnt=write(msgsock, buf, 1024)) == -1)
		        printf("socket write Error !\n");
	       	    memset(buf,0,(sizeof buf));
		}
		/* File wird geloescht */
        	if (access("load.tmp", 0) != -1)
        	{
                	/* printf(" remove File !!! \n"); */
                	remove("load.tmp");
        	}
	  }
       }


      /* Der Client schickt "getLoad" --> Lastangabe schicken */
      if ( strncmp (EXEC_MSG, befehl, strlen(EXEC_MSG))==0)
      {
	memset(syscall,0,(sizeof syscall));
        strcpy(syscall, para);
        printf("RemoteSystemCall: %s\n", syscall);
        strcat(syscall, " > result.tmp");
        /* printf("systemcall: >%s\n", syscall); */
        system(syscall);

        /* File zu lesen oeffnen */
	if (( fd = open("result.tmp", O_RDONLY)) == -1 )
        {
		/* im Fehlerfall Meldung schicken */
		memset(buf,0,(sizeof buf));
		sprintf(buf, "Can't open result.tmp !");
		printf("%s", buf);
		write(msgsock, buf, strlen(buf));
		write(msgsock, EOT, strlen(EOT));
         }
         else
         {
                /* sonst das File Blockweise uebertragen */
                memset(buf,0,(sizeof buf));
                while( (rd_cnt = read(fd, buf, (sizeof buf) -1)) > 0)
                {
                     /* printf("send %i bytes to client !\n", rd_cnt); */
                     if ( (wrt_cnt=write(msgsock, buf, 1024)) == -1)
                             printf("socket write Error !\n");
                     memset(buf,0,(sizeof buf));
                }
                /* Ende signalisieren */
                write(msgsock, EOT, strlen(EOT));

		/* File wird geloescht */
        	if (access("result.tmp", 0) != -1)
        	{
                	/* printf(" remove File !!! \n"); */
                	remove("result.tmp");
        	}
          }
       }

   } while (TRUE); /* diese Schleife wird durch breaks bei kill
		   und bye verlassen */
   close(msgsock); /* die Verbindung zum Clienten abbauen */
} 
 
/* Diese Funktion erzeugt einen Thread Das Argument ist der
   Socketdescriptor fuer dir Kommunikation mit dem Clienten */
void create_thread(int msgsock)
{
        thread_t t;
        PThreadArg p;
        /* Specher fuer den Parameter anlegen */
        p = (PThreadArg)malloc(sizeof(ThreadArg));
	/* Parameter in das struct schreiben */
        p->msock=msgsock;
	/* den Thread erzeugen */
        thr_create(NULL,0,thread_func,(void *)p,THR_NEW_LWP,&t);
}


void main( int argc, char *argv[])
{
 struct sockaddr_in server;
 int sock, msgsock, port;
 /* das erste Argument wird als Pfad genommen */
 if (argc > 1) 
 {
	if (strncmp(argv[1], HELP_ARG, strlen(HELP_ARG)) == 0)
	{
	   	showHelp(argv[0]);
	}
        /* wurde ein Argument angegeben wird diese als Port interpretiert */
	port = atoi(argv[2]); /* den Port festlegen */
 }
 else 
	port = 0;

 printf("\nComputeServer --> weitere Informationen mit %s %s\n\n", argv[0], HELP_ARG);
 /* TCP - Verbindung aufbauen */
 sock = InitTCP(&server, port);

 /* den Socket Empfangsbereit machen  */
 listen( sock, 5 );
  do
  {
     /* auf Anfragen der Clienten warten */
     msgsock = accept( sock, (struct sockaddr *)0, (int *)0 );
     if (msgsock == -1)
       perror("accept");
     else
       /* aus der Anfrage einen Thread erzeugen */
       create_thread(msgsock);
  } while(TRUE); /* Dies Schleife wird durch ein kill aus dem 
		    Thread abgebrochen */
 close(sock);
} 
