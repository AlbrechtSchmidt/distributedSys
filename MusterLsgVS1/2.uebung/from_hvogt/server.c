/* H. Vogt */
/* Verteilte Systeme  WS 94/95 */
/* Date/Time-Server */

/* 2. Uebungsblatt, Aufgabe 4 */

#include "service.h"

  /* Zwischenraum zwischen zwei gesendeten 'alive'-Signalen */
const long aliveGap = 2;
  /* Timeout, um den 'Tod' des Partner-Servers festzustellen */
const long deadGap = 15;

  /* Pipes, ueber die die Kommunikation mit dem Partner-Server laeuft */
int pipes[2];

  /* die Variable 'whoami' enthaelt einen Wert, der angibt, ob man selbst */
  /* die Rolle des Haupt-Servers oder des Control-Servers spielt */
#define C_SERVER 0
#define C_CONTROL 1
int whoami;

int run_server(void);
int run_control(void);

void create_server(void) {
  int c;

  printf("*** creating new server ***\n");
  c = fork();
  if (c == 0) {
    whoami = C_SERVER;
    run_server();
  }
  else printf("pid = %d\n", c);
}

void create_control(void) {
  int c;

  printf("*** creating new control server ***\n");
  c = fork();
  if (c == 0) {
    whoami = C_CONTROL;
    run_control();
  }
  else printf("pid = %d\n", c);
}

int work(struct Request *req, struct Answer *answ) {
  time_t currentTime;
  struct tm cTime;

  currentTime = time(NULL);
  localtime_r(&currentTime, &cTime);


  switch (req->type) {
    case 1:
      sprintf(answ->timeString, "Zeit: %02d:%02d:%02d", cTime.tm_hour,
        cTime.tm_min, cTime.tm_sec);
      break;
    case 2:
      sprintf(answ->timeString, "Datum: %02d.%02d.%04d", cTime.tm_mday,
        cTime.tm_mon+1, cTime.tm_year+1900);
      break;
    case 3:
      ctime_r(&currentTime, answ->timeString, 50);
      break;
  }
/*
  currentTime = time(NULL);
  ctime_r(&currentTime, answ->timeString, 50);
*/
  return TRUE;
}

int otherSideAlive(void) {
  struct stat pipestat;
  char eat[20];
  long w;

  fstat(pipes[whoami], &pipestat);
  w = pipestat.st_size;
  DDEBUG(printf("[%ld]", w);)
  if (w > 20) w = 20;
    /* Konsumiere moeglichst viel von dem, was auf der Pipe ist */
  read(pipes[whoami], eat, w);
    /* Falls etwas auf der Pipe war, lebt der andere */
  return (w > 0);
}

int sendAliveSig(void) {
  write(pipes[whoami], "X", 1);
  DDEBUG(printf("<%d>", whoami);)
}

  /* Folgende Funktion haelt den Server am Leben */
void keep_alive(void) {
  static time_t lastTime = 0;
  time_t nowTime;
  static long myTime = 0;
  static long otherTime = 0;

    /* Initialisierung beim ersten Aufruf */
  if (lastTime == 0) lastTime = time(NULL);

    /* Feststellen der vergangenen Zeit seit letztem Aufruf */
  nowTime = time(NULL);
  myTime += (nowTime - lastTime);
  otherTime += (nowTime - lastTime);
  lastTime = nowTime;

  DDEBUG(printf("myTime: %ld  otherTime: %ld\n", myTime, otherTime);)

    /* Sende eigenes Alive-Signal */
  if (myTime > aliveGap) {
    sendAliveSig();
    myTime = 0;
  }

    /* Teste, ob Partner-Server noch lebt */
  if (otherSideAlive()) {
    otherTime = 0;
  }
  else if (otherTime > deadGap) {
    if (whoami == C_SERVER)
      create_control();
    else
      create_server();
    otherTime = 0;
  }

}
  
int run_server(void) {
  struct Request r;
  struct Answer a;

  while (TRUE) {

    while (!request_pending()) {
      printf("Warte auf Anfrage...\n");
      keep_alive();
      sleep(2);
    }

    printf("Bearbeite Anfrage\n");
    read_request(&r);
    work(&r, &a);
    send_answer(&a);
    keep_alive();

    while (request_pending()) {
      printf("Warte, dass Antwort abgeholt wird...\n");
      keep_alive();
      sleep(2);
    }

    clear_answer();

  }
}

int run_control(void) {
  int cnt = 0;

  while (TRUE) {
    keep_alive(); 
    DDEBUG(printf(".");)
    sleep(3);
    cnt++;
    if (cnt > 100) return 1;
  }
}

int run(void) {

    /* Erzeuge Pipes */
  pipe(pipes);

    /* Erzeuge einen Haupt-Server, laufe selbst als Control-Server */
  create_server();
  whoami = C_CONTROL;
  run_control();
}

main() {
  open_connection();
  run();
  close_connection();
}

