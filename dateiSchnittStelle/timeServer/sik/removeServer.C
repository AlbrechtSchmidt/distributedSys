#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "KommuDIR.h"

void write_remove_Request()
{
  FILE *file_nr;
  file_nr = fopen("Request.kill", "w+");
  fclose(file_nr);
  chmod("Request.kill", S_IRWXO | S_IRWXU);
  printf("\nsend Request.kill to Server\n\n" );
}


int main()
{
  chdir(KommuDIR);
  write_remove_Request();
  return 0;
}
