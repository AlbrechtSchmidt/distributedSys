#include <stdio.h>

#include "service.h"

void stoperror(char *str) {
  printf("%s\n", str);
  printf("error no: %d\n", errno);
  exit(1);
}

void status(void) {
/*
  int result;
  lseek(medium_file, 0, SEEK_SET);
  result = lockf(medium_file, F_TEST, 1);
  printf("req: %d ", result);
  lseek(medium_file, 1, SEEK_SET);
  result = lockf(medium_file, F_TEST, 1);
  printf("answ: %d\n", result);
*/
}

int request_pending(void) {
  int result;
  status();
  lseek(medium_file, REQ_LOCK_AREA, SEEK_SET);
  result = lockf(medium_file, F_TEST, LOCK_SIZE);
  return (result == -1 ? TRUE : FALSE);
}

int answer_pending(void) {
  int result;
  status();
  lseek(medium_file, ANSW_LOCK_AREA, SEEK_SET);
  result = lockf(medium_file, F_TEST, LOCK_SIZE);
  return (result == -1 ? TRUE : FALSE);
}

int send_request(struct Request *r) {
  int result;

  lseek(medium_file, REQ_LOCK_AREA, SEEK_SET);
  result = lockf(medium_file, F_TLOCK, LOCK_SIZE);
  /* printf("locking result request: %d\n", result); */

  lseek(medium_file, REQ_AREA, SEEK_SET);
  write(medium_file, r, REQ_SIZE);

  return (result == 0);
}

int send_answer(struct Answer *a) {
  int result;

  lseek(medium_file, ANSW_LOCK_AREA, SEEK_SET);
  result = lockf(medium_file, F_TLOCK, LOCK_SIZE);
  /* printf("locking result answer: %d\n", result); */

  lseek(medium_file, ANSW_AREA, SEEK_SET);
  write(medium_file, a, ANSW_SIZE);

  return (result == 0);
}

int read_request(struct Request *r) {
  lseek(medium_file, REQ_AREA, SEEK_SET);
  read(medium_file, r, REQ_SIZE);
}

int read_answer(struct Answer *a) {
  lseek(medium_file, ANSW_AREA, SEEK_SET);
  read(medium_file, a, ANSW_SIZE);
}

int clear_answer(void) {
  int result;
  lseek(medium_file, ANSW_LOCK_AREA, SEEK_SET);
  result = lockf(medium_file, F_ULOCK, LOCK_SIZE);
  return (result == 0);
}

int clear_request(void) {
  int result;
  lseek(medium_file, REQ_LOCK_AREA, SEEK_SET);
  result = lockf(medium_file, F_ULOCK, LOCK_SIZE);
  return (result == 0);
}

int open_connection(void) {
  medium_file = open(MEDIUM, O_RDWR | O_NDELAY | O_CREAT, 0666);
  if (medium_file == -1) stoperror("medium file could not be opened");
}

int close_connection(void) {
  close(medium_file);
}

