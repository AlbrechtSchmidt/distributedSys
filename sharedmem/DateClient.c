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
	int *Time;					/* Pointer auf die Msg.- */
	char *clientwrite, *clientread, *Request;	/* Struktur im SharedMem.*/
	
	/* Zum Testen kann nach 0777 auch "| IPC_CREAT" eingefuegt werden */
	/* und der Client vor dem Server gestartet werden. Was passiert?? */
	if ( ( shmid_w= shmget(SHMKEY_W, K, 0777 )) == -1)
		printf("Client_Write failure");
	if ( (shmid_r = shmget(SHMKEY_R, K, 0777 )) == -1)
		printf("Client_Read failure");
		
	clientwrite = shmat(shmid_r, 0, 0);
	clientread  = shmat(shmid_w, 0, 0);
	
	printf(" Der Client fordert im 1. Bereich ab 0x %x an \n
		sein Datum findet er nach: 0x %x\n", clientwrite, clientread);
	
	Request = (char *) clientwrite;			/* einfacher Request */
	*Request = 'R';
	
	Time = (int *) clientread;			/* Antwort und       */
	while (*Time < 1)				/* "Verarbeitung     */
		wait(5);
	printf("Die Uhrzeit ist: %i, und somit ist es 5 vor 12 Uhr!", *Time);

	
	if (shmdt(clientwrite) < 0)
		printf("client write detach");		/* Der Shared Memory - Ber. */
	if (shmdt(clientread) < 0)			/* wird wieder abgemeldet,   */
		printf("client read detach");
		
	shmctl(shmid_r, IPC_RMID,0);			/* und schliesslich wieder  */
	shmctl(shmid_w, IPC_RMID,0);			/* freigegeben              */

	/*Zur Beobachtung: Die letzten beiden Zeilen ausquoten und neu testen,*/
	/*auch mal im Server und in unterschiedl. Reihenfolge startem         */
}	

