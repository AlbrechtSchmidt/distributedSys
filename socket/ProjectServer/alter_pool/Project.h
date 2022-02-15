#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define TRUE      1
#define DIR_MSG "dir"
#define GET_MSG "get"
#define PUT_MSG "put"
#define CLOSE_MSG "bye"
#define KILL_MSG  "kill"

#define EOT ">EOT<"
#define MODE  0000666  /* Dateizugriff */


