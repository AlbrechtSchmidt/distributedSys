/* Compilieren mit:  /usr/ucb/cc timemess.c -lm */

#include <sys/time.h>
#include <sys/resource.h>
#include <math.h>

int who;
struct rusage stop;

main()
{

 int i;
 float r;

 getrusage(who, &stop); 
/* holt die Userzeit und die Systemzeit */
 
 printf("stop s.sec  %i \n",stop.ru_stime.tv_sec);
 printf("stop s.usec %i \n",stop.ru_stime.tv_usec);
 printf("stop u.sec  %i \n",stop.ru_utime.tv_sec);
 printf("stop u.usec %i \n",stop.ru_utime.tv_usec);
 
 for(i=1;i<10000;i++) r=sqrt(i);

 getrusage(who, &stop);
/* holt die Userzeit und die Systemzeit */
 printf("stop s.sec  %i \n",stop.ru_stime.tv_sec);
 printf("stop s.usec %i \n",stop.ru_stime.tv_usec);
 printf("stop u.sec  %i \n",stop.ru_utime.tv_sec);
 printf("stop u.usec %i \n",stop.ru_utime.tv_usec);
 
 for(i=1;i<10000;i++) r=sqrt(i);
 for(i=1;i<10000;i++) r=sqrt(i);
 for(i=1;i<10000;i++) r=sqrt(i);
 for(i=1;i<10000;i++) r=sqrt(i);

 getrusage(who, &stop);
/* holt die Userzeit und die Systemzeit */
 printf("stop s.sec  %i \n",stop.ru_stime.tv_sec);
 printf("stop s.usec %i \n",stop.ru_stime.tv_usec);
 printf("stop u.sec  %i \n",stop.ru_utime.tv_sec);
 printf("stop u.usec %i \n",stop.ru_utime.tv_usec);
 
 for(i=1;i<10000;i++) r=sqrt(i);

 getrusage(who, &stop);
/* holt die Userzeit und die Systemzeit */
 printf("stop s.sec  %i \n",stop.ru_stime.tv_sec);
 printf("stop s.usec %i \n",stop.ru_stime.tv_usec);
 printf("stop u.sec  %i \n",stop.ru_utime.tv_sec);
 printf("stop u.usec %i \n",stop.ru_utime.tv_usec);

} 
