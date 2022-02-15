#include <stdio.h>
#include <signal.h>

void ErrorHandler(strPtr) char* strPtr;{printf("Error: %s\n",strPtr); exit(-1);}

void UserSignalHandler(sig) int sig; { 
     signal(sig,UserSignalHandler);
     printf("PID = %d: Signal %d im SignalHandler angekommen\n",getpid(),sig);
     sleep(2); /* only for debugging */
}

main() {
   int  childpid, parentpid, retcode;
   do {
	signal(SIGCLD,SIG_IGN);
	signal(SIGUSR1,UserSignalHandler);

	if ((childpid = fork()) < 0) ErrorHandler("can`t fork");

	if (childpid > 0) {  
	  /* parent-process */
	  do {
	    retcode = kill(childpid,0 /* SIGUSR1 */);
	    printf("Vater pid = %d erhaelt zurueck: %d\n",getpid(),retcode);
            if (retcode < 0 ) {
               printf(" -->> den Sohn neu aufstarten\n");
	       break;
	    }
	    sleep(2);
	  } while (1);
	} else { 
	  /* child-process */
	  parentpid = getppid();
	  do {
	    retcode = kill(parentpid,0 /* SIGUSR1 */);
	    printf("Sohn  pid = %d erhaelt zurueck: %d\n",getpid(),retcode);
	    if (retcode < 0 ) {
	       printf(" -->> den Vater neu aufstarten\n");
	       break;
	    }
	    sleep(2);
	  } while(1);
        }
   } while (1);
}
