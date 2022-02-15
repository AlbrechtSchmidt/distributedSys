#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>


#define TRUE      1
#define DIR_MSG "dir" 
#define GET_MSG "get" 
#define CLOSE_MSG "bye" 
#define EOT ">EOT<" 
#define KILL_MSG  "kill" 
#define MAXFILES  10
#define MODE  0000666

 
 
main( int argc, char *argv[])
{
  int sock,i, ready, fd, rd_cnt, wrt_cnt;
  struct sockaddr_in server;
  struct hostent *hp, *gethostbyname();
  char buf[1024], cmd[50], LocalName[200];

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
  
  do
  {
    printf("\n>");
    gets(cmd);
    if (strncmp(cmd, KILL_MSG, strlen(KILL_MSG)) == 0)
    {
	if ( write(sock, KILL_MSG, sizeof KILL_MSG) == -1 )
 	    perror("writing on stream socket");
	break;
    }

    if (strncmp(cmd, CLOSE_MSG, strlen(CLOSE_MSG)) == 0)
    {
	if ( write(sock, CLOSE_MSG, sizeof CLOSE_MSG) == -1 )
 	    perror("writing on stream socket");
	break;
    }

    if (strncmp(cmd, DIR_MSG, strlen(DIR_MSG)) == 0)
    {
	if ( write(sock, DIR_MSG, sizeof DIR_MSG) == -1 )
 	    perror("writing on stream socket");
	printf("\nDirectory \n");
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

    if (strncmp(cmd, GET_MSG, strlen(GET_MSG)) == 0)
    {
	printf(" lokaler Dateiname: ");
	gets(LocalName);

        if (( fd = open(LocalName, O_WRONLY | O_CREAT, MODE)) == -1 )
                {
                        printf("Can't open %s !\n", LocalName);
                }
	else 
	{
           if ( write(sock, cmd , strlen(cmd)) == -1 )
               perror("writing on stream socket");
           printf(" downloading file  ");

           do
           {
                if (read(sock, buf, sizeof buf) == -1 )
                  perror("reading on stream socket");
                if (strncmp(EOT, buf, strlen(EOT))== 0) ready =1;
                else
                {
                        ready=0;
                	if ( (wrt_cnt = write(fd, buf, strlen(buf))) == -1)
			{
                            printf("Can't write in file %s !\n", LocalName);
                	}
		        printf(".");
                }
            }
            while(!ready);
	    printf("\n");
	}
    }
  }
 
  while(TRUE);
  close(sock);
  printf("\n... bye !\n");
  exit(0);
}

     
