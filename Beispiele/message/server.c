/* ------------------  message/server.c ------------------ */

#include "vertsys.h"

main() {
	int n, filefd, mesqid;
	MyMessageType mesg;
	char errmesg[256];

	/* Create the message queue */
	if ((mesqid=msgget(MKEY,PERMS|IPC_CREAT)) <0)
		ErrorHandler("server can`t get message queue\n");
	do {
	   /* Read the filename message from the IPC descriptor */
	   mesg.mesg_type = REQUEST;
	   if ((n=mesg_recv(mesqid, &mesg)) <= 0)
		ErrorHandler("Server filename read error!\n");
	   mesg.mesg_data[n] = '\0';
	   mesg.mesg_type = RESPONSE;
	   if ((filefd = open(mesg.mesg_data,0)) < 0 ) {
		strcat(mesg.mesg_data,": Server can't open this file!\n");
		/* send an error-message back */
		mesg.mesg_len = strlen(mesg.mesg_data);
		mesg_send(mesqid, &mesg);
	   } else {
		/* Read the data from the file */
		while ((n=read(filefd,mesg.mesg_data,MAXMESGDATA)) > 0 ) {
			mesg.mesg_len = n;
			mesg_send(mesqid, &mesg);
		}
		close(filefd);
		if (n<0) printf("Server read error from file!");
	   }
	   /* Send a message with a length of 0 to signify the end */
	   mesg.mesg_len =0;
	   mesg_send(mesqid, &mesg);
	} while (1);
}
 
