/* ------------------  message/client.c ------------------ */

#include 	"vertsys.h"

main(argc,argv) int argc; char * argv[]; {
	int n, mesqid;
	char * strPtr = "Welche Datei soll angezeigt werden: ";
	MyMessageType mesg;
	
	if ((mesqid=msgget(MKEY,0)) <0)
		ErrorHandler("Client can`t msgget");

	if (argc <2 ) {	
		write(1,strPtr,strlen(strPtr));
		fgets(mesg.mesg_data, MaxMesgDATA,stdin);
	} else 	strcpy(mesg.mesg_data,argv[1]);

	n = strlen(mesg.mesg_data);
	if (mesg.mesg_data[n-1] == '\n') n--;
	mesg.mesg_data[n] = '\0';
	mesg.mesg_datalen = n;
	mesg.mesg_type = REQUEST;
	mesg.mesg_pid = getpid();
	mesg_send(mesqid,&mesg);

	mesg.mesg_type = (long) getpid();
	while (n=mesg_recv(mesqid, &mesg))
		write(1,mesg.mesg_data,n);
	if (n<0) printf("Data read error\n");
}

