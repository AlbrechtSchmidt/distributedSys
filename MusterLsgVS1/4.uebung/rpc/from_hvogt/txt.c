/* text client */

#include <stdio.h>
#include <rpc/rpc.h>
#include "txt.h"

main(int argc, char *argv[])
{
  CLIENT *cl;
  static int r;
  char *val;
  char **rval;
 
  val = (char *)malloc(MAX_STR);

  if (argc < 3) {
    fprintf(stderr, "usage is: %s <server> <command>\n", argv[0]);
    fprintf(stderr, "  where <command> = a |\n");
    fprintf(stderr, "                    s <TextNo>\n");
    exit(1);
  }

  if (!(cl = clnt_create(argv[1], TXTPROG, TXTVERS, "tcp"))) {
    clnt_pcreateerror(argv[1]);
    exit(1);
  }

  switch (argv[2][0]) {
    case 'a':
      printf("enter text: "); gets(val);
      r = *addelement_1(&val, cl);
      printf("return code is %d\n", r);
      break;
    case 's':
      r = atoi(argv[3]);
      printf("asking for text element %d...\n", r);
      rval = showelement_1(&r, cl);
      printf("returned...\n");
      strcpy(val, *rval);
      printf("text %d is: %s\n", r, val);
      break;
  }
}

