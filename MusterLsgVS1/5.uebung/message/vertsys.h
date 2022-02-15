/* ------------------  message/vertsys.h ------------------ */

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/msg.h>

#define MKEY	 1234L
#define PERMS	 0666
#define REQUEST	 1L

#define MaxMESG		128
#define MesgPID		sizeof(int)
#define MaxMesgDATA	(MaxMESG-MesgPID)

typedef struct {
	int   mesg_datalen;
	long  mesg_type;
	int   mesg_pid;
	char  mesg_data[MaxMesgDATA];
} MyMessageType;

