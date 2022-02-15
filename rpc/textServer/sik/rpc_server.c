#include <rpc/rpc.h>
#include "text.h"
#include <signal.h>
#include <sys/types.h>

#define MaxText 100
#define MaxLen 50

char* a[MaxText]; /* 100 Zeiger auf Texte */
int fillNr=0;

void * initarray_1()
{
  int i;
  char buf[60];
  for(i=0;i<MaxText;i++)
  {
    sprintf(buf, "Text No %i", i);
    strcat(buf," -- not filled yet !! --");
    a[i]=(char *) malloc ( strlen(buf) );
    strcpy(a[i], buf);
  }
  fillNr=0;
}

int * serverpid_1()
{
   int static i;
   i = getpid();
   return(&i);
}

char ** showelement_1(Nr)  int * Nr; 
{
        static char *ch;
        ch = (char *) malloc( MaxLen );
        if ((*Nr) < MaxText) ch=strcpy(ch ,a[(*Nr)]);
             else strcpy(ch, " Out of Range !!! ");
	return(&ch);
}

int * addelement_1(text) char ** text;
{
    int static i;
    if (fillNr < MaxText)
    {
       a[fillNr]=(char *) malloc ( strlen(*text) );
       strcpy(a[fillNr], *text);
       i=fillNr;
       fillNr++;
    }
    else i=-1;
    return(&i);
}
