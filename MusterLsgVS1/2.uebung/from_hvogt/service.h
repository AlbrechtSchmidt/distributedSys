/* service.h */
/* gemeinsame Datenstrukturen fuer client/server */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0

#define MEDIUM "MEDIUM_FILE"

/*
  Aufbau des Medium-Files:

  pos=0 len=1   locked=request available
  pos=1 len=1   locked=answer available
  pos=2 len=REQ_SIZE   request data
  pos=2+REQ_SIZE len=ANSW_SIZE  answer data
*/

  /* Request types */
  /* type 1 = time request */
  /* type 2 = date request */
  /* type 3 = time & date request */
struct Request {
  int type;
};

struct Answer {
  char timeString[100];
};

#define REQ_SIZE (sizeof(struct Request))
#define ANSW_SIZE (sizeof(struct Answer))

/* Konstanten fuer den Aufbau des Medium-Files */

#define LOCK_SIZE 1
#define REQ_LOCK_AREA 0
#define ANSW_LOCK_AREA 1
#define REQ_AREA (REQ_LOCK_AREA + ANSW_LOCK_AREA)
#define ANSW_AREA (REQ_AREA + REQ_SIZE)

int medium_file;

void stoperror(char *str);
 
int request_pending(void);
int answer_pending(void); 
int send_request(struct Request *r);
int send_answer(struct Answer *a);
int read_request(struct Request *r);
int read_answer(struct Answer *a);
int clear_answer(void);
int clear_request(void);

int open_connection(void);
int close_connection(void);

/* #define DDEBUG_ON 1 */
#ifdef DDEBUG_ON
#define DDEBUG(x) x
#else
#define DDEBUG(x) 
#endif
