/* H. Vogt */
/* Verteilte Systeme  WS 94/95 */
/* Date/Time-CLient */

/* 2. Uebungsblatt, Aufgabe 4 */

#include "service.h"

int do_request(int n) {
  char t[20];
  struct Request r;
  struct Answer a;

  r.type = n;

  while (request_pending() || answer_pending()) {
    printf("Fremde Anfrage laeuft noch...\n");
    sleep(1);
  }

  printf("Sende eigene Anfrage\n");
  send_request(&r);

  while (!answer_pending()) {
    printf("Warte auf Anwort...\n");
    sleep(1);
  }

  printf("Antwort angekommen. Mit <Eingabe> uebernehmen."); gets(t);

  read_answer(&a);
  clear_request();

  printf("Antwort: %s\n", a.timeString);
}

void run(void) {
  char t[20];
  int r;

  while (TRUE) {
    printf("\n\nWelche Anfrage soll gesendet werden?\n\n");
    printf("1 = Zeit\n");
    printf("2 = Datum\n");
    printf("3 = Zeit+Datum\n");
    printf("[0=Ende] > ");
    gets(t);

    r = atoi(t);
    if (r >= 1 && r <= 3)
      do_request(r);
    else if (r == 0) break;
  }
}

main() {
  open_connection();
  run();
  close_connection();
}
