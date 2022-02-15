#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PAGESIZE 4096

int fd;
caddr_t result;


void segv(int a)
{
	caddr_t x;

printf("SIGSEGV signal\n");
x = mmap((void *)result, PAGESIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, fd, 0);
}

main()
{
	char * p;
	char c;

if ((fd = open("/dev/zero", O_RDWR)) == -1) return 1;
result = mmap((void *)0, PAGESIZE, PROT_READ, MAP_SHARED, fd, 0);
printf("address = %08X\n",result);

p=(char *)result;
signal(SIGSEGV,segv);

c=*p;
printf("read %d\n",c);

*p=1;
printf("write %d\n",*p);
return 0;
}
