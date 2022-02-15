#include "vertsys.h"

void SayServerIs(fileName) char * fileName; {
    creat (fileName,0666);
}

void WaitForClient(fileName) char * fileName; {
    while (unlink(fileName) == -1) sleep(1);
}

void CreateResponse() {
    struct timeval timeVal; FILE *stream;
    stream = fopen("response","w");
    gettimeofday(&timeVal);
    fprintf(stream,ctime(&timeVal.tv_sec));
    fclose(stream);
}

main (argc, argv) int argc; char *argv[]; {
    if (argc > 1) chdir(argv[1]);
    SayServerIs("waiting");
    do { WaitForClient("request");
	 CreateResponse();
	 SayServerIs("ready");
    } while (TRUE);
}
