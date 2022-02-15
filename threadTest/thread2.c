#include <stdio.h>
#include <thread.h>
#include <string.h>

struct SInfoBlock {
	char name[80];
	int delay; 
	};

typedef struct SInfoBlock TInfoBlock;
typedef struct SInfoBlock * PInfoBlock;

void * thread_func(void * arg)
{
	PInfoBlock pi = (PInfoBlock)arg;
	while(1) {
		printf("##### thread %s\n",pi->name);
		sleep(pi->delay);
		}
}


void create_thread(char * name, int delay)
{
	thread_t t;
	PInfoBlock p;

	p = (PInfoBlock)malloc(sizeof(TInfoBlock));
	strcpy(p->name,name); p->delay=delay;
	thr_create(NULL,0,thread_func,(void *)p,THR_NEW_LWP,&t);
}

main()
{
	int i;

	create_thread("abc",1);
	create_thread("xyz",3);

	for (i=0; i<20; ++i) {
		printf("main %d\n",i);
		sleep(1);
		}
	return 0;
}

