#include <stdio.h> 
#include <fcntl.h> 
#include <sys/time.h> 
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/stat.h>

#define FALSE 0
#define TRUE  1
#define AND &&
#define OR ||

#define SERVER_PORT 7777

