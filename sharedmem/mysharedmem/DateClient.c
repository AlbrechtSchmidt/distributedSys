#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>

#define SHMKEY_R 50
#define SHMKEY_W 51
#define K 2

int shmid_w;
int shmid_r;

main()
{
	int *Msg, *NullSet;				/* Pointer auf die Msg.- */
	int *clientwrite, *clientread, *Request;	/* Struktur im SharedMem.*/
        int i;
    	
	/* Zum Testen kann nach 0777 auch "| IPC_CREAT" eingefuegt werden */
	/* und der Client vor dem Server gestartet werden. Was passiert?? */
	if ( ( shmid_w= shmget(SHMKEY_W, K, 0777 )) == -1)
		printf("Client_Write failure");
	if ( (shmid_r = shmget(SHMKEY_R, K, 0777 )) == -1)
		printf("Client_Read failure");
		
	clientwrite = shmat(shmid_r, 0, 0);
	clientread  = shmat(shmid_w, 0, 0);
	
	printf(" Der Client schreibt Nr ab 0x %x \n", clientwrite); 
	printf(" und liest in 	           0x %x\n",  clientread);


     for(i=0;i<30;i++)
     {	
	Msg = (int *) clientwrite;
        printf("\n Msg vor Req is : %i \n", *Msg);

        Request = (int *) clientwrite;			/* einfacher Request */
	*Request = i;
        sleep(1);
       	
	Msg = (int *) clientread;			/* Antwort und       */

	
        printf("\n Request is    : %i ", *Request);
        printf("\n Msg     is    : %i \n", *Msg);
   
        
        NullSet = (int *) clientread;
        *NullSet = 0;
     }




	
	if (shmdt(clientwrite) < 0)
		printf("client write detach");		/* Der Shared Memory - Ber. */
	if (shmdt(clientread) < 0)			/* wird wieder abgemeldet,   */
		printf("client read detach");
		
	shmctl(shmid_r, IPC_RMID,0);			/* und schliesslich wieder  */
	shmctl(shmid_w, IPC_RMID,0);			/* freigegeben              */

	/*Zur Beobachtung: Die letzten beiden Zeilen ausquoten und neu testen,*/
	/*auch mal im Server und in unterschiedl. Reihenfolge startem         */
}	

