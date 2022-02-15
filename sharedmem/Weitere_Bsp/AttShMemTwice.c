#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>

#define SHMKEY 75
# define K 1024
int shmid;

main()
{
	int i, *pint;
	char *addr1, *addr2;
	
	extern cleanup();
	
	for(i=0; i<20; i++)
		signal(i,cleanup);
	shmid = shmget(SHMKEY, 128 * K, 0777 | IPC_CREAT);
	addr1 =shmat(shmid, 0, 0);
	addr2 =shmat(shmid, 0, 0);
	printf("addr1 0x%x addr2 0x %x\n", addr1, addr2);
	pint = (int *) addr1;
	
	for (i=0; i<256; i++)
		*pint++ = i;
	pint = (int*) addr1+1;
	*pint= 256;
	
	pint = (int *) addr2;
	for (i=0; i<256; i++)
		printf("index %d\tvalue %d\n",i, *pint++);
		
		pause(5);
		
}


cleanup()
{
	shmctl(shmid,IPC_RMID,0);
	printf("cleanup shmid%d\n", shmid);
	
	exit();
}
