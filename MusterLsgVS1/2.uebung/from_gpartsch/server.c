#include "vertsys.h"

void SayServerIs(fileName) char * fileName; {
    creat (fileName,0666);
}

void ErrorHandler(strPtr) char * strPtr; { 
     printf("Error: %s",strPtr); exit(-1);
}

void CreateResponse() {
    struct timeval timeVal; FILE *stream;
    stream = fopen("response","w");
    gettimeofday(&timeVal);
    fprintf(stream,"%s\n",ctime(&timeVal.tv_sec));
    fclose(stream);
}

void TimeoutSignalHandler(n){
    signal(SIGALRM,TimeoutSignalHandler);
}

int TestPipe(fd) int fd; {
int n; char buff[10];
	alarm(2); n=0;
	if((n=read(fd,buff, sizeof(buff))) <= 0) {
		printf("Error: TestPipe\n"); return(FALSE); }
	alarm(0);
	return (TRUE);
}

main() {
int childpid, pipe1[2],pipe2[2];

Label_CreateDateAndControlServer:
    creat("medium",0666);
    signal(SIGPIPE,SIG_IGN);
    signal(SIGALRM,TimeoutSignalHandler);
    if (pipe(pipe1) <0 || pipe(pipe2)<0) ErrorHandler("create pipes\n");  
    if ((childpid=fork()) <0) ErrorHandler("fork\n");

    if (childpid >0) { /* parent-process -> DateAndTimeServer */
             close(pipe1[0]); close(pipe2[1]);
             do { 
	        while (unlink("request") == -1) { /* WaitForClient("request");*/
		    /* in die Control-Pipe schreiben */
		    if (write(pipe1[1],"1",1) != 1) printf("write pipe1[1] error\n");
                    sleep(1);
		    if (TestPipe(pipe2[0]) == FALSE) {
	        	/* -> Control-Server neu aufstarten */
                  	close(pipe1[1]); close(pipe2[0]);
	     		goto Label_CreateDateAndControlServer;
		    }
                }
	        CreateResponse();
	        SayServerIs("ready");
             } while (TRUE);

    } else {  /* child-process -> Control-Server */
	     close(pipe1[1]); close(pipe2[0]);
             do { 
		if (write(pipe2[1],"1",1) != 1) printf("write pipe2[1] error\n");
		sleep(1);
             } while (TestPipe(pipe1[0]));
	     /* -> Date-Server neu aufstarten */
             close(pipe1[0]); close(pipe2[1]);
	     goto Label_CreateDateAndControlServer;
    }
}
