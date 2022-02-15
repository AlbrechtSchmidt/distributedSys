#include <stdio.h>
#include <thread.h>

#define BSIZE 82

sema_t empty,full;     

struct {
	char data[BSIZE];
	int size;
	} buf[2];


void * producer(void * arg)
{
	int i=0;

	while(1) {
		sema_wait(&empty);
		buf[i].size = read(0, buf[i].data, BSIZE);
printf(" pro: %i \n", buf[i].size);
		sema_post(&full);
		if (buf[i].size <= 0) break;
		i ^= 1;
	}
}

void * consumer(void * arg)
{
	int i=0;

	while(1) {
		sema_wait(&full);
printf(" con: %i \n", buf[i].size);
		if (buf[i].size <= 0) break;
		write(1, buf[i].data, buf[i].size);
		sema_post(&empty);
		i ^= 1;
		}
}


main()
{
	thread_t tproducer, tconsumer;

	sema_init(&empty,2,USYNC_THREAD,NULL);
	sema_init(&full,0,USYNC_THREAD,NULL);
	thr_create(NULL,0,producer,NULL,THR_NEW_LWP,&tproducer);
	thr_create(NULL,0,consumer,NULL,THR_NEW_LWP,&tconsumer);
	thr_join(tconsumer,NULL,NULL);


	return 0;
}

