/*****************************************************
 *                                                   *
 * mytelnet.c --- simples telnet                     *
 * testet den simple name server sns                 *
 *                                                   *
 *****************************************************/

#include <stdio.h>

#include "nameclient.h"

main(int argc, char **argv) {
	char	comstr[256];
	char	*addrptr;

	if (argc != 2) {
		fprintf(stderr,"Usage: %s <host>\n",argv[0]);
		exit(1);
	}

	if ( (addrptr=Lookup(argv[1])) == NULL ) {
		perror("Can't Lookup host");
		exit(1);
	}

	sprintf(comstr,"telnet %s",addrptr);

	system(comstr);

	printf("Thanx for using mytelnet\n");

}
