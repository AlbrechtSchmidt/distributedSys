#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ParentCHAR	'p'
#define ChildCHAR	's'
#define ShmemKEY	1234567	/* numerical SharedMem-Name */

void ErrorHandler(strPtr) char* strPtr;{printf("Error: %s\n",strPtr); exit(-1);}

main() {
   int  i, childpid, shmid;
   char ch, *shmemPtr,*shPtr;
   do {
	shmid = shmget(ShmemKEY,1,0666|IPC_CREAT);
	shmemPtr = (char *) shmat(shmid,0,0);

	if ((childpid = fork()) < 0) ErrorHandler("can`t fork");

	if (childpid > 0) {  /* parent-process */
	  *shPtr = ParentCHAR;
   	  for (i=0;i++;i<2) { 
		ch = *shPtr;
		printf("Vater pid = %d erhaelt zurueck: %c\n",getpid(),ch);
		if (ch = ChildCHAR) {
	   	   *shPtr = ParentCHAR; i=0; }
		sleep(3);
   	  }
          printf(" -->> den Sohn neu aufstarten\n");
	} else {     	/* child-process */
	  *shmemPtr = ChildCHAR;
   	  for (i=0;i<2;i++) { 
		ch = *shmemPtr;
		printf("Sohn  pid = %d erhaelt zurueck: %c\n",getpid(),ch);
		if (ch == ParentCHAR) {
	   	   *shmemPtr = ChildCHAR; i=0; }
		sleep(3);
   	  }
          printf(" -->> den Vater neu aufstarten\n");
	}
   } while (1);
}
