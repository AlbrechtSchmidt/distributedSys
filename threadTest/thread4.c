#include <stdio.h>
#include <thread.h>

void * thread_func(void * arg)
{
	while(1) {
		printf("*********  thread\n");
		sleep(2);
		}
}


main()
{
	thread_t t;
	int i;

	thr_create(NULL,0,thread_func,NULL,NULL,&t);
	for (i=0; i<20; ++i) {
		printf("main %d\n",i);
		sleep(1);
		}
	return 0;
}

