/* Compilieren mit:  /usr/ucb/cc timemess.c -lm */

#include <sys/types.h>
#include <sys/timeb.h>
#include <math.h>


struct timeb now;

main()
{
 int i;
 float r;

 
 ftime(&now); /* holt die aktuelle Zeit */
 
 printf(" time     %i \n",now.time);
 printf(" millitm  %i \n",now.millitm);
 printf(" timezone %i \n",now.timezone);
 printf(" dstflag  %i \n",now.dstflag);


 /* for(i=1;i<10000;i++) r=sqrt(i); */

 ftime(&now); /* holt die aktuelle Zeit */
 
 printf(" time     %i \n",now.time);
 printf(" millitm  %i \n",now.millitm);
 printf(" timezone %i \n",now.timezone);
 printf(" dstflag  %i \n",now.dstflag);
 
 /* for(i=1;i<1000;i++) r=sqrt(i); */
 
 ftime(&now); /* holt die aktuelle Zeit */
  
 printf(" time     %i \n",now.time); 
 printf(" millitm  %i \n",now.millitm); 
 printf(" timezone %i \n",now.timezone); 
 printf(" dstflag  %i \n",now.dstflag); 
  

}
