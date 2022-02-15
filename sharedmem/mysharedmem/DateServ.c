#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include 	<time.h>

#define SHMKEY_R 50
#define SHMKEY_W 51

#define K 1024
int shmid_r;
int shmid_w;
time_t t;

main()
{
	int  *Msg, *NullSet;			/* Pointer auf die Msg.- */
	int  *servread, *servwrite, *Request;	/* Struktur im SharedMem.*/
      	
        int i;
 	
	 /* Shared Memory - Bereich wird erzeugt */
	if ( (shmid_r = shmget(SHMKEY_R, K, 0777 | IPC_CREAT)) == -1)
		printf("Server_Read failure");
	if ( ( shmid_w= shmget(SHMKEY_W, K, 0777 | IPC_CREAT)) == -1)
		printf("Server_Write failure");
		
	 /* Die Startadresse des Shared Memory - Bereich wird erst durch */
	 /* Aufruf von shmat zurueckgeliefert*/
	servread = shmat(shmid_r, 0, 0);
	servwrite =shmat(shmid_w, 0, 0);
	
      
        printf(" Der Server schreibt Nr ab 0x %x \n", servwrite); 
        printf(" und liest in              0x %x\n",  servread);
 
	
     for(i=0;i<30;i++) 
     {   
        Msg = (int *) servwrite; 
        printf("\n Msg vor Req is : %i \n", *Msg);
  


        Request = (int *) servwrite;                  /* einfacher Request */
        *Request = i; 
        sleep(1);

        Msg = (int *) servread;                       /* Antwort und       */
         
        printf("\n Request is : %i ", *Request); 
        printf("\n Msg     is : %i \n", *Msg);
    
 
        NullSet = (int *) servread;
        *NullSet = 0;
 
     }
 

	
	
	if (shmdt(servread) < 0)		/* Der Shared Memory - Ber. */
		printf("server read detach");	/* wird wieder abgemeldet,   */
	if (shmdt(servwrite) < 0)
		printf("server write detach");

	shmctl(shmid_r, IPC_RMID,0);		/* und schliesslich wieder  */
	shmctl(shmid_w, IPC_RMID,0);		/* freigegeben              */

}
