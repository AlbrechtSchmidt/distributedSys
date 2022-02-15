/* this is the source for the text server */

/* this file was NOT created by rpcgen... */

#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "txt.h"

#define LIST_SIZE 100

text_record TextList[LIST_SIZE];
int TextNo = 0;

int * addelement_1(char **s)
{
  static int status;

  if (TextNo < LIST_SIZE) {
    TextList[TextNo].element_text = (char *)malloc(MAX_STR);
    strcpy(TextList[TextNo].element_text, *s);
    status = TextNo;
    TextNo++;
  }
  else
    status = -1;
  return ((int *)&status);
}

char ** showelement_1(int *n)
{
  static char *retText = NULL;

  if (retText == NULL) {
    retText = (char *)malloc(MAX_STR);
  }

  strcpy(retText, "<empty>");
  if (*n < TextNo && *n >= 0) {
    strcpy(retText, TextList[*n].element_text);
  }
  return (&retText);
}

