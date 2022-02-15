#include <rpc/rpc.h>
#include <string.h>
#include "date.h"

#define ZEILEN   10
#define SPALTEN  100

char text[ZEILEN][SPALTEN];
char * charPtr;
int  maxLine = 0;

int * add_element_1(strHandle) char ** strHandle; {
	int static i;
	strcpy(text[maxLine],*strHandle);
	i = maxLine;
	maxLine = (maxLine+1) % ZEILEN;
	return(&i);
}

char ** show_element_1(line)  int *line; {

	charPtr=&text[*line][0];
	return(&charPtr);
}

