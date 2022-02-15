#include <stdio.h>
#include <signal.h>
#include "k.h"

main(int argc, char* argv[])
{
 if ( kill(atoi(argv[1]), MY_SIG_CODE ) )
   printf("still alive !!!\n");
  else printf("off !!!\n");
} 
