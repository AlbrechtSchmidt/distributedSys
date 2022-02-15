#include <stdio.h>
#include <thread.h>
#include <string.h>

struct SInfoBlock {
	char name[80];
	int delay; 
	int oft;
	};

typedef struct SInfoBlock TInfoBlock;
typedef struct SInfoBlock * PInfoBlock;

void * thread_func(void * arg)
{
        int i;
	PInfoBlock pi = (PInfoBlock)arg;
	for(i=1;i < (pi->oft); i++) 
		{
		printf("##### thread %s\n",pi->name);
		sleep(pi->delay);
		}
}


void create_thread(char * name, int delay, int oft)
{
	thread_t t;
	PInfoBlock p;

	p = (PInfoBlock)malloc(sizeof(TInfoBlock));
	strcpy(p->name,name);
	p->delay=delay;
	p->oft=oft;
	thr_create(NULL,0,thread_func,(void *)p,THR_NEW_LWP,&t);
}

main()
{
	int i;

	create_thread("abc",1, 10);
	create_thread("xyz",3, 5);

	for (i=0; i<30; ++i) {
		printf("main %d\n",i);
		sleep(1);
		}
	return 0;
}

