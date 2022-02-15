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
	int  *Serv;				/* Pointer auf die Msg.- */
	char *servread, *servwrite, *Request;	/* Struktur im SharedMem.*/
	
	
	 /* Shared Memory - Bereich wird erzeugt */
	if ( (shmid_r = shmget(SHMKEY_R, K, 0777 | IPC_CREAT)) == -1)
		printf("Server_Read failure");
	if ( ( shmid_w= shmget(SHMKEY_W, K, 0777 | IPC_CREAT)) == -1)
		printf("Server_Write failure");
		
	 /* Die Startadresse des Shared Memory - Bereich wird erst durch */
	 /* Aufruf von shmat zurueckgeliefert*/
	servread = shmat(shmid_r, 0, 0);
	servwrite =shmat(shmid_w, 0, 0);
	
	printf("ReadAddr: 0x%x   WriteAddr: 0x%x\n",servread, servwrite);
	
		
	Request = (char *) servread; 		/* Anforderungen vorhanden? */
	
	while (	*Request != 'R')		/* Nicht schoen, spart aber */
		sleep(1);			/* Platz auf der Folie!     */

	printf("Request erhalten um: ");
	time(&t);
   	printf(ctime(&t));
	
	Serv = (int *) servwrite;		/* Senden der Pseudo-Zeit   */
	*Serv = 1155;
	sleep(2); 				/* s.o.                     */
	
	
	if (shmdt(servread) < 0)		/* Der Shared Memory - Ber. */
		printf("server read detach");	/* wird wieder abgemeldet,   */
	if (shmdt(servwrite) < 0)
		printf("server write detach");

	shmctl(shmid_r, IPC_RMID,0);		/* und schliesslich wieder  */
	shmctl(shmid_w, IPC_RMID,0);		/* freigegeben              */

}
