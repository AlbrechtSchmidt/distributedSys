#include <stdio.h>

FILE *file;
long ticks;

main()
{
   file = fopen("medium", "r+");

   while(1)
   {
     flockfile(file);
     rewind(file);
     fscanf(file, "%15ld", &ticks);
     fflush(file);
     funlockfile(file);
     printf("Client: %15ld\n", ticks);
     sleep(1);
   }
}
