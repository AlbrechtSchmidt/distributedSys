#include <stdio.h>
#include <signal.h>
#include "k.h"

#define X (int (*)())-1

/*
void (*f)();
*/

main()
{
   long n=0;
   float x=99999;

   if (signal(MY_SIG_CODE, SIG_IGN) == X) printf("sig-err\n");

   printf("PID : %i \n", getpid());

   do
   {
     n++;
     x=x*(x/n)*n;
     if (n % 100000 == 0) printf(".");
   }
   while(1==1);
}
