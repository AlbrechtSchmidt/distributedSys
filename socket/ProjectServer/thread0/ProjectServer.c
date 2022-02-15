#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define S_PORT 7891


#define TRUE      1
#define DIR_MSG "dir" 
#define GET_MSG "get" 
#define CLOSE_MSG "bye" 
#define EOT ">EOT<" 
#define KILL_MSG  "kill" 
#define MAXFILES  10

typedef struct
{	int msock;
} ThreadArg;

typedef ThreadArg * PThreadArg;


typedef struct 
{	char Alias[20];
	char Path[60];
} DirEntry;


DirEntry  FileTable[MAXFILES];
int entries, sock;

int InitTCP(struct sockaddr_in * server)
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
  (*server).sin_port = htons(S_PORT);
  /* if ( bind( sockId, (struct sockaddr *)&server, sizeof server ) == -1 ) */
  if ( bind( sockId, (struct sockaddr *) server, sizeof (*server) ) == -1 )
  {
     perror("binding stream socket");
     exit(1);
  }

  length = sizeof server;
  /* if ( getsockname( sockId, (struct sockaddr *)&server, &length ) == -1) */
  if ( getsockname( sockId, (struct sockaddr *)server, &length ) == -1)
  {
     perror("getting socket name");
     exit(1);
  }
  printf("Socket installed on Port #%d\n", ntohs( (*server).sin_port));
  return sockId;
}

void * thread_func(void * arg)
{
  PThreadArg p = (PThreadArg)arg;
  char buf[1024], befehl[30], para[100], ch; 
  int rval, para_num, i, msgsock;
  int fd, rd_cnt, wrt_cnt;
  msgsock = p->msock;
  do 
  { 
      memset(buf, 0, sizeof buf);    
      if ((rval = read(msgsock, buf, 1024)) == -1)
            perror("reading stream message");
      if (strncmp(CLOSE_MSG, buf, strlen(CLOSE_MSG))==0)
      { 
                break;
      } 
      if (strncmp(KILL_MSG, buf, strlen(KILL_MSG))==0)
      { 
                 close(msgsock);
		 close(sock);	
		 exit(1);
      }    
      if (rval == 0)    
           printf("Ending connection\n");
      else 
      { 
            para_num=sscanf(buf, "%s%s", befehl, para);
            if (para_num < 1)
            { 
                strcpy(befehl, "");
                strcpy(para,"");
            } 
            if (para_num == 1)
            { 
                strcpy(para,"");
            } 
            printf("-->Befehl: %s %s \n", befehl, para);
            
            /* directory schicken ! */
            if ( strncmp (DIR_MSG, befehl, strlen(DIR_MSG))==0)
            {
               for(i=0;i<=entries;i++)
               {
               		sprintf(buf,"(%i) : %s\n", i, FileTable[i].Alias);
            		write(msgsock, buf, sizeof buf);
               }
	       write(msgsock, EOT, strlen(EOT));
            }

            /* angefordertes file schicken */
            if ( strncmp (GET_MSG, befehl, strlen(DIR_MSG))==0)
	    {
                i=atoi(para);
		if (( fd = open(FileTable[i].Path, O_RDONLY)) == -1 )
	        {
			memset(buf,0,(sizeof buf));
			sprintf(buf, "Can't open %s ! (file: %s)\n",
				FileTable[i].Alias, FileTable[i].Path);
			printf("%s", buf);
			write(msgsock, buf, strlen(buf));
			write(msgsock, EOT, strlen(EOT));
                }
                else
                {
			while( (rd_cnt = read(fd, buf, sizeof buf)) > 0)
			{
				printf("send %i bytes to client !\n", rd_cnt);
				memset(buf+rd_cnt,0,(sizeof buf)-rd_cnt);
				if ( (wrt_cnt=write(msgsock, buf, 1024)) == -1)
				  printf("socket write Error !\n");
			}
			write(msgsock, EOT, strlen(EOT));
		}
	    }
      } 
   } while (rval != 0);
   close(msgsock);
} 
 


int InitDir()
{
  strcpy(FileTable[0].Alias, "AliasName0"); strcpy(FileTable[0].Path,  "PathName0");
  strcpy(FileTable[1].Alias, "AliasName1"); strcpy(FileTable[1].Path,  "PathName1");
  strcpy(FileTable[2].Alias, "AliasName2"); strcpy(FileTable[2].Path,  "PathName2");
  strcpy(FileTable[3].Alias, "AliasName3"); strcpy(FileTable[3].Path,  "PathName3");
  strcpy(FileTable[4].Alias, "AliasName4"); strcpy(FileTable[4].Path,  "PathName4");
  strcpy(FileTable[5].Alias, "AliasName5"); strcpy(FileTable[5].Path,  "PathName5");
  entries = 5;
}

void create_thread(int msgsock)
{
        thread_t t;
        PThreadArg p;

        p = (PThreadArg)malloc(sizeof(ThreadArg));
        p->msock=msgsock;
        thr_create(NULL,0,thread_func,(void *)p,THR_NEW_LWP,&t);
}


main()
{
 int i, rval, para_num;
 struct sockaddr_in server;
 int msgsock;
 char buf[1024], befehl[30], para[100];
 InitDir();
 sock = InitTCP(&server);

 listen( sock, 5 );
  do
  {
     msgsock = accept( sock, (struct sockaddr *)0, (int *)0 );
     if (msgsock == -1)
       perror("accept");
     else
       create_thread(msgsock);

  } while(TRUE);
 close(sock);
} 
