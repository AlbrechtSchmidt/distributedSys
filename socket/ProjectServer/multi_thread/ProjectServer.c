/*  ********************************************************** 
    Uebung Verteilte Systeme SS95             Albrecht Schmidt
    
    Programmieraufgabe Blatt 5: Project File Server
                                  
                                PFS - Server

    Der PFS-Server ist Thread-basierend, er laesst sich also
    nur auf Solaris (neuer Pool) compilieren.
 
    Datum 10.7.95                                    Version 1 
    ********************************************************** */ 
 

#include "Project.h" 
#include <signal.h>     /* kill() wird benoetigt um aus dem Thread das 
                           Programm zu beenden */  
#include <thread.h>     /* fuer die Threads */
#include <dirent.h>     /* zum lesen des File-Directory */
 
#define STD_PATH "./"   /* Standartpfad falls kein Argument angegeben wird */
#define MAXFILES  100   /* Anzahl maximal zulaessiger Directory-Eintraege */
#define HELP_ARG  "-help"
 

typedef struct	        /* Parameter fuer die Threadfunktion in struct */
{	                /* speichern .., falls noch weiter Parameter dazu kommen */
	int msock;
} ThreadArg;

typedef ThreadArg * PThreadArg; 


typedef struct          /* struct der Verzeichniseintraege */
{	char Alias[40];
	char Path[200];
} DirEntry;


DirEntry  FileTable[MAXFILES]; /* das Directory */
int entries;	     /* Anzahl belegter Directory-Eintraege */
char KommuDIR[256];  /* Hier steht das publizierte Verzeichnis */


int showHelp(char * Pname)
{
  printf("\nUSAGE: %s [<directory> [<port>]]\n\n", Pname);
  printf("Wird <port> nicht angegeben waehlt das System einen aus !\n");
  printf("Wird <directory> und <port> nicht angegeben wird auf dem\n");
  printf("Standartdirectory %s gearbeitet und das System waehlt\n", STD_PATH); 
  printf("einen Port aus !\n\n");
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

/* Funktion zum Verzeichnis lesen */
void readDIR()
{
 
   DIR *dirp;
   struct dirent *direntp;
 
   entries =0;
   /* Verzeichnis oeffnen */
   dirp = opendir( KommuDIR );
     /* solange noch was da ist lesen und die Maximalzahl nicht
        ueberschritten ist lesen */
     while ( ( (direntp = readdir( dirp )) != NULL ) || (entries > MAXFILES))
     {
         /* nur files anzeigen die nicht mit einem "." beginnen ! */
         if (direntp->d_name[0] != '.')
         {
           strcpy(FileTable[entries].Alias, direntp->d_name);
           strcpy(FileTable[entries].Path, KommuDIR);
           strcat(FileTable[entries].Path, direntp->d_name);
           entries++;
         }
      }
   closedir( dirp );
}   
 


/* Die thread-Funktion, sie bearbeitet die Anfragen */
void * thread_func(void * arg)
{
  PThreadArg p = (PThreadArg)arg; /* Parameter in Variablem umwandeln */
  char buf[1024], befehl[30], para[100]; 
  int rval, para_num, i, l, msgsock, ready;
  int fd, rd_cnt, wrt_cnt;
  msgsock = p->msock; /* locale Variable mit Parameter belegen */
  do 
  { 
      memset(buf, 0, sizeof buf);    
      /* Anfrage lesen vom Socket */
      if ((rval = read(msgsock, buf, 1024)) == -1)
            perror("reading stream message");
     
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

      /* sonst wenn eine andere Meldung ankommt */
      if (rval == 0)    
           printf("Ending connection\n");
      else 
      { 
	    /* Message in Befehl und Parameter zerlegen */
            para_num=sscanf(buf, "%s%s", befehl, para);
            if (para_num < 1) /* leere Befehl */
            { 
                strcpy(befehl, "");
                strcpy(para,"");
            } 
            if (para_num == 1) /* Befehl ohne Parameter */
            { 
                strcpy(para,"");
            } 
            printf("-->Befehl: %s %s \n", befehl, para);
            
            /* Der Client schickt "dir" --> directory schicken ! */
            if ( strncmp (DIR_MSG, befehl, strlen(DIR_MSG))==0)
            {
 	       readDIR(); /* aktuelles Verzeichnis einlesen */
               /* alle Eintraege schicken */
	       for(i=0;i<entries;i++)
               {
               		sprintf(buf,"(%.2i) : %s\n", i, FileTable[i].Alias);
            		write(msgsock, buf, sizeof buf);
               }
	       /* Ende signalisieren */
	       write(msgsock, EOT, strlen(EOT));
            }

            /* Der Client schickt "get ?" --> angefordertes file schicken */
            if ( strncmp (GET_MSG, befehl, strlen(GET_MSG))==0)
	    {
                i=atoi(para); /* String in Zahl umwandeln 
			         --> Nummer des Files */
	        /* File zu lesen oeffnen */
		if (( fd = open(FileTable[i].Path, O_RDONLY)) == -1 )
	        {
			/* im Fehlerfall Meldung schicken */
			memset(buf,0,(sizeof buf));
			sprintf(buf, "Can't open %s ! (file: %s)\n",
				FileTable[i].Alias, FileTable[i].Path);
			buf[1024]='\0';
			printf("%s", buf);
			write(msgsock, buf, strlen(buf));
			write(msgsock, EOT, strlen(EOT));
                }
                else
                {
			/* sonst das File Blockweise uebertragen */
			memset(buf,0,(sizeof buf));
			while( (rd_cnt = read(fd, buf, sizeof buf)) > 0)
			{
				printf("send %i bytes to client !\n", rd_cnt);
				if ( (wrt_cnt=write(msgsock, buf, 1024)) == -1)
				    printf("socket write Error !\n");
				memset(buf,0,(sizeof buf));
			
			}
	       		/* Ende signalisieren */
			write(msgsock, EOT, strlen(EOT));
		}
	    }



            /* Der Client schickt "put ?" --> geschicktes file speichern */
            if ( strncmp (PUT_MSG, befehl, strlen(PUT_MSG))==0)
            {
                i=atoi(para); /* String in Zahl umwandeln
                                 --> Nummer des Files */

		/* File wird geloescht */
        	if (access(FileTable[i].Path, 0) != -1)
        	{
                	printf(" remove old File !!! \n");
                	remove(FileTable[i].Path);
        	}

                /* File zu schreiben oeffnen */
                if (( fd = open(FileTable[i].Path, O_WRONLY | O_CREAT, MODE)) == -1 )
                {
                        /* im Fehlerfall Meldung ausgeben */
        		printf("Can't open %s !\n", FileTable[i].Path);
                }
                else
                {
			/* File blockweise lesen und in die Datei schreiben */
           		do
           		{
                	   memset(buf, 0, sizeof buf);
                	   if (read(msgsock, buf, sizeof buf) == -1 )
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
                            	   printf("Can't write in file %s !\n", FileTable[i].Path);
                        	}
                        	printf(" ."); /* pro Block einen Punkt schreiben */
                	    }
            		}
            		while(!ready); /* Schleife beenden wenn EOT vom Server kommt */
            		printf("\n");
                }
            }
      } 
   } while (rval != 0); /* Ende wenn eine 0 Byte Nachricht empfangen wurde
			   diese Schleife wird auch durch breaks bei kill
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
	else 
	{
		strcpy(KommuDIR, argv[1]);
        	/* das lezte Zeichen muss ein "/" sein ! */ 
		if (KommuDIR[strlen(KommuDIR)-1] != '/') strcat(KommuDIR, "/");
	}
  }
 /* wird kein Pfad als Argument angegeben wird der Standardpfad genommen */
 else strcpy(KommuDIR, STD_PATH);
  
 /* wurde ein 2. Argument angegeben wird diese als Port interpretiert */
 /* sonst wird vom System ein Port (server.port = 0) gewaehlt         */
 if (argc > 2) 
	port = atoi(argv[2]); /* den Port festlegen */
 else 
	port = 0;

 printf("\nProjectServer --> weitere Informationen mit %s %s\n\n", argv[0], HELP_ARG);
 /* TCP - Verbindung aufbauen */
 sock = InitTCP(&server, port);
 printf("Directory is: %s\n\n" , KommuDIR);

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
