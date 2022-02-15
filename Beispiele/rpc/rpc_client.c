#include <stdio.h>
#include <rpc/rpc.h>
#include "date.h"

main(argc, argv)  int argc; char * argv[]; {

	CLIENT	*cl;
	char	*server;
	long	*lresult;
	char	**sresult;

	if (argc != 2) { fprintf(stderr,"Aufruf: %s hostname\n", argv[0]);
		exit(1);
	}
	server = argv[1];

	if ((cl = clnt_create(server,MY_DATE_PROG,DATE_VERS,"udp")) == NULL) {
		clnt_pcreateerror(server);
		exit(2);
	}

	if ((lresult = bin_date_1(NULL,cl)) == NULL ) {
		clnt_perror(cl,server);
		exit(3);
	}
	printf("Seconds since 1.1.70  \"%s\": %ld\n",server, *lresult);

	if ((sresult = str_date_1(lresult,cl)) == NULL) {
		clnt_perror(cl,server);
		exit(4);
	}
	printf("Date and Time on Host \"%s\": %s",server,*sresult);

	clnt_destroy(cl);
	exit(0);
}

