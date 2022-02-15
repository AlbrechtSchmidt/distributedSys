all: thread1 thread2 thread3

thread4: thread4.c
	cc -othread4 -D_REENTRANT thread4.c -lthread

thread1: thread1.c
	cc -othread1 -D_REENTRANT thread1.c -lthread

thread2: thread2.c
	cc -othread2 -D_REENTRANT thread2.c -lthread

thread3: thread3.c
	cc -othread3 -D_REENTRANT thread3.c -lthread
