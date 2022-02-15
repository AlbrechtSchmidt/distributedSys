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
	   printf("Request from Client with PID = %d   Requested Filename = %s\n",mesg.mesg_pid,mesg.mesg_data);

	   mesg.mesg_type = (long) mesg.mesg_pid;
	   if ((filefd = open(mesg.mesg_data,0)) < 0 ) {
		strcat(mesg.mesg_data,": Server can't open this file!\n");
                printf("%s",mesg.mesg_data);
		/* send an error-message back */
		mesg.mesg_datalen = strlen(mesg.mesg_data);
		mesg_send(mesqid, &mesg);
	   } else {
		/* Read the data from the file */
		while ((n=read(filefd,mesg.mesg_data,MaxMesgDATA)) > 0 ) {
			mesg.mesg_datalen = n;
			mesg_send(mesqid, &mesg);
			printf("%d ",n);
		}
		close(filefd);
		if (n<0) printf("Server read error from file!");
	   }
	   /* Send a message with a length of 0 to signify the end */
	   mesg.mesg_datalen =0;
	   printf("0\n"); 
	   mesg_send(mesqid, &mesg);
	} while (1);
}
 
