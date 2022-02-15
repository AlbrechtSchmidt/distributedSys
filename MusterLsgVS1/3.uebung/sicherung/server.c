#include "vertsys.h"

void ErrorHandler(strPtr) char* strPtr; {printf("Error: %s",strPtr); exit(-1);}

void TimeoutSignalHandler(n) {signal(SIGALRM,TimeoutSignalHandler);}

int TestPipe(fd) int fd; {
	int n; char buff[10];
	alarm(2); n=0;
	if ((n=read(fd, buff, sizeof(buff))) <= 0) {
		printf("Error: Server-Control-Pipe!\n"); return (FALSE); }
	alarm(0);
	return (TRUE);
}

main() {
	struct timeval  timeVal;
	int             childpid, fd1, fd2, sd, i, first=TRUE;
	char            buf[20], *bufPtr;
	struct sockaddr_in client, server;

   do {
	signal(SIGPIPE, SIG_IGN);
	signal(SIGALRM, TimeoutSignalHandler);
	unlink("S-C.pipe"); unlink("C-S.pipe");

	if (mknod("S-C.pipe", S_IFIFO | S_IRUSR | S_IWUSR, 0) < 0)
		ErrorHandler("mknod S-C.pipe\n");
	if (mknod("C-S.pipe", S_IFIFO | S_IRUSR | S_IWUSR, 0) < 0)
		ErrorHandler("mknod C-S.pipe\n");

	if ((childpid = fork()) < 0) ErrorHandler("fork\n");

	if (childpid > 0) {	/* parent-process -> DateAndTimeServer */
		if ((fd1 = open("S-C.pipe", O_WRONLY)) == -1)
			ErrorHandler("open S-C.pipe\n");
		if ((fd2 = open("C-S.pipe", O_RDONLY)) == -1)
			ErrorHandler("open C-S.pipe\n");
		if (first) {
		  sd = socket(AF_INET, SOCK_DGRAM, 0);
		  server.sin_family = AF_INET;
		  server.sin_addr.s_addr = htonl(INADDR_ANY);
		  server.sin_port = htons(SERVER_PORT);
		  if( bind(sd, (struct sockaddr *) & server, sizeof(server)) != NULL)
    			perror("Bind-Error");
		  fcntl(sd, F_SETFL, FNDELAY);
		  first = FALSE;
		}

		do {
			bzero(buf, sizeof(buf));
			i = sizeof(client);
			if (recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *) & client, &i) != -1) {
				/* Hier Fallunterscheidung nach Date, Time
				   und DateAndTime einbauen */
				gettimeofday(&timeVal);
				bufPtr = ctime(&timeVal.tv_sec);
				sendto(sd, bufPtr, strlen(bufPtr) + 1, 0, (struct sockaddr *) & client, sizeof(client));
			} else sleep(1);

			/* in die Control-Pipe schreiben */
			write(fd1,"1",1); sleep(1);
			if (TestPipe(fd2) == FALSE) {
				/* -> Control-Server neu aufstarten */
				close(fd1); close(fd2);
				break;
			}
		} while (TRUE);

	} else {		/* child-process -> Control-Server */
		if ((fd2 = open("S-C.pipe", O_RDONLY)) == -1)
			ErrorHandler("open S-C.pipe\n");
		if ((fd1 = open("C-S.pipe", O_WRONLY)) == -1)
			ErrorHandler("open C-S.pipe\n");

		do {
			write(fd1,"1",1); sleep(1);
		} while (TestPipe(fd2));
		/* -> Date-Server neu aufstarten */
		close(fd1); close(fd2);
	}
   } while (TRUE);
}
