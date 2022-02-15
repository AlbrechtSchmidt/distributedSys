/* ------------------  message/message.c ------------------ */

#include	"vertsys.h"

void ErrorHandler(strPtr) char* strPtr;{printf("Error: %s\n",strPtr);exit(-1);}

void mesg_send(mesqid, mesgptr) int mesqid; MyMessageType *mesgptr; {

	if(msgsnd(mesqid,(struct msgbuf *) &(mesgptr->mesg_type),mesgptr->mesg_len,0) != 0)
		printf("Error in function mesg_send()\n");
}

int mesg_recv(mesqid, mesgptr) int mesqid; MyMessageType *mesgptr; {
	int n;
	n = msgrcv(mesqid, (struct msgbuf *) &(mesgptr->mesg_type), MAXMESGDATA,mesgptr->mesg_type,MSG_NOERROR);

/*n=n-sizeof(int);
*/
	if ((mesgptr->mesg_len = n) < 0)
		printf("Error in function mesg_recv\n");
	return(n);
}

