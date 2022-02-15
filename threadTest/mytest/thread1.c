#include <stdio.h>
#include <thread.h>

void * thread_func(void * arg)
{
	int i;
	for(i=1;i<8;i++)
		{
		printf("*********  thread\n");
		sleep(2);
		}

}


main()
{
	thread_t t;
	int i;

	thr_create(NULL,0,thread_func,NULL,THR_NEW_LWP,&t);
	for (i=0; i<20; ++i) {
		printf("main %d\n",i);
		sleep(1);
		}
	return 0;
}

