#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define TRUE      1
#define GET_MSG "getLoad"
#define EXEC_MSG "remoteExec"
#define CLOSE_MSG "bye"
#define KILL_MSG  "kill"

#define EOT ">EOT<"


