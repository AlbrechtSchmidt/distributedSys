/* ------------------  message/vertsys.h ------------------ */

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/msg.h>

#define MKEY	 1234L
#define PERMS	 0666
#define REQUEST	 1L
#define RESPONSE 2L

#define MAXMESGDATA	(4096-16)
#define MESGHDRSIZE	(sizeof(MyMessageType)-MAXMESGDATA)

typedef struct {
	int   mesg_len;
	long  mesg_type;
	char  mesg_data[MAXMESGDATA];
} MyMessageType;

