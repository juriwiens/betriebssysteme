/**
Betriebssysteme Blatt 4, Aufgabe 4-3

Datei: a4-3.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 19.05.2015

Aufgabe:
Main thread soll zweiten thread erzeugen und auf Eingabe von Enter-Taste warten.
Erfolgt die Eingabe, dann soll der zweite thread beendet werden. Der zweite
thread läuft in einer Endlosschleife, in der er 5 Sekunden schläft und eine
Meldung ausgibt.

Nutzungshinweise:
Aufruf ohne Argumente.

Verwendete Systemfunktionen:
  int pthread_cancel(pthread_t thread) :
  Schickt thread eine Abbruchanforderung.

  Quellen: man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.

Frage:
Kann der zweite thread periodisch seine Meldung ausgeben während der main
thread auf Eingabe von der Tastatur wartet?

Antwort: Ja, ich sehe keinen Grund warum dies nicht funktionieren sollte.

Frage:
Kann der main thread seine Aufgabe erfüllen, Enter von der Tastatur
entgegenzunehmen, den zweiten thread terminieren und eine Meldung ausgeben
während der zweite thread sich gerade am Anfang eines 5 sec Schlafes befindet?

Antwort: Ja, ich sehe keinen Grund warum dies nicht funktionieren sollte.
*/

#include <stdio.h> // printf, perror
#include <pthread.h> // pthread_create, pthread_join, pthread_detach
#include <stdbool.h> // bool data type
#include <unistd.h> // sleep

pthread_t second_thread;

/**
* Creates thread with default attributes and handles errors.
*/
void create_default_thread(pthread_t *thread_ptr, void *(*func_ptr)(void *),
                          void *arg_ptr, bool detached) {
  int res;

  res = pthread_create(thread_ptr, NULL, func_ptr, arg_ptr);
  if (res != 0) {
    perror("ERROR: pthread_create failed\n");
  }

  // Detach thread, if flag is set
  if (detached == true) {
    res = pthread_detach(*thread_ptr);
    if (res != 0) {
      perror("ERROR: pthread_detach failed\n");
    }
  }
}

/**
* Processes second thread.
*/
void *process_second_thread(void *arg) {
  while(true) {
    sleep(5);
    printf("[second] I slept 5 seconds\n");
  }

  pthread_exit(NULL);
}

/**
* Waits for user to press enter and kills second thread.
*/
void prompt_user() {
  char input[2];

  printf("Press enter to exit:\n");
  fgets(input, 2, stdin);

  if (pthread_cancel(second_thread) == 0) {
    printf("Cancelled second thread successfully");
  } else {
    perror("ERROR: Cancelling second thread failed");
  }

  printf("%s\n", input);
}

int main(void) {
  // Create second thread
  create_default_thread(&second_thread, process_second_thread, NULL, true);

  prompt_user();

  return 0;
}
