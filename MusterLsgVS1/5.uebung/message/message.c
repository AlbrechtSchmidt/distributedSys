/* ------------------  message/message.c ------------------ */

#include	"vertsys.h"

void ErrorHandler(strPtr) char* strPtr;{printf("Error: %s\n",strPtr);exit(-1);}

void mesg_send(mesqid, mesgptr) int mesqid; MyMessageType *mesgptr; {
int len;
len = mesgptr->mesg_datalen+sizeof(int);

	if(msgsnd(mesqid,(char *) &(mesgptr->mesg_type),(mesgptr->mesg_datalen+MesgPID),0) != 0)
		printf("Error in function mesg_send()\n");
}

int mesg_recv(mesqid, mesgptr) int mesqid; MyMessageType *mesgptr; {
	int n;
	n = msgrcv(mesqid, (char *) &(mesgptr->mesg_type), MaxMESG,mesgptr->mesg_type,0);
	n = n - MesgPID;
	if ((mesgptr->mesg_datalen = n) < 0)
		printf("Error in function mesg_recv\n");
	return(n);
}

