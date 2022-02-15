#include <stdio.h>
#include <rpc/rpc.h>
#include "date.h"

main(argc, argv)  int argc; char * argv[]; {

	CLIENT	*cl;
	char	*server, *charPtr, **stringPtr, stdText[80];
	int	i, *intPtr, lineNr;

	if (argc < 2) { fprintf(stderr,"Aufruf: %s hostname\n", argv[0]);
		exit(1);
	}
	server = argv[1];

	if ((cl = clnt_create(server,TEXT_SERVER,TEXT_VERS,"udp")) == NULL) {
		clnt_pcreateerror(server);
		exit(2);
	}
	
	if (argc < 3 ) {
	   strcpy(stdText,"Uebungsaufgabe zur Vorlesung Verteilte Systeme I");
	   charPtr = stdText;
	}
	else charPtr = argv[2];
	if ((intPtr = add_element_1(&charPtr,cl)) == NULL ) {
		clnt_perror(cl,server);
		exit(3);
	}
	lineNr = *intPtr;
	printf("Angehaengte Textzeile:\n%d. %s\n", lineNr+1,charPtr);

	printf("Ausgabe aller Textelemente von Text-Server \"%s\"\n",server);
	for (i=0;i<=lineNr;i++) {
	     *intPtr = i;
	     if ((stringPtr = show_element_1(intPtr,cl)) == NULL) {
		clnt_perror(cl,server);
		exit(4);
	     }
	     printf("%d. %s\n",(*intPtr)+1,*stringPtr);
	}

	clnt_destroy(cl);
	exit(0);
}

