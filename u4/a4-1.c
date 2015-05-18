/**
Betriebssysteme Blatt 4, Aufgabe 4-1

Datei: a4-1.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 18.05.2015

Aufgabe:
Programm schreiben, welches zwei Threads erzeugt und auf deren Beendigung
wartet. In dem einen thread soll ein Zeichen ’x’ n1 mal, in dem anderen ein
Zeichen ’y’ n2 mal nach stdout ausgegeben werden.

Nutzungshinweise:
Aufruf ohne Argumente.

Verwendete Systemfunktionen:
  int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
      void *(*start_routine)(void *), void *arg) :
  Erzeugt einen neuen Thread und generiert eine eindeutige ID, welche in thread
  geschrieben wird. Mit Hilfe von attr können bestimmte Attribute wie z.B. die
  Priorität des Threads übergeben werden, bei NULL werden Standardwerte gesetzt.
  Mit dem start_routine Parameter wird eine Referenz auf eine Funktion
  übergeben, die vom Thread ausgeführt werden soll. Diese muss void* zurückgeben
  und exakt ein void* Argument annehmen. Dieses eine Argument wird mittels des
  arg Zeigers übergeben. Gibt bei erfolgreicher Erzeugung 0, ansonsten eine
  Fehlernummer zurück.

  int pthread_join(pthread_t thread, void **value_ptr) :
  Blockiert die Ausführung des ausführenden Threads bis der als Argument
  übergebene thread terminiert. Der Rückgabewert des Threads wird hierbei in
  value_ptr geschrieben. Gibt bei Erfolg 0, sonst eine Fehlernummer zurück.

  Quellen: man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdio.h> // required by printf, perror
#include <pthread.h> // required by pthread_create, pthread_join

/**
* Number of threads to create.
*/
const int THREAD_COUNT = 2;

/**
* Parameter struct, which can be handed over to char_print().
*/
typedef struct {
  char character; // Char to be printed
  int char_count; // Number of chars to be printed
} char_print_params_t;

/**
* Creates thread with default attributes and handles errors.
*/
void create_default_thread(pthread_t *id_ptr, void *(*func_ptr)(void *),
                          void *arg_ptr) {
  int res;

  res = pthread_create(id_ptr, NULL, func_ptr, arg_ptr);
  if (res != 0) {
    perror("ERROR: pthread_create failed");
  }
}

/**
* Prints a number of a character, both definied in params_ptr.
*/
void *char_print(void *params_ptr) {
  // Cast params_ptr from void* to char_print_params_t*
  char_print_params_t *params = (char_print_params_t *) params_ptr;

  int i;
  for (i = 0; i < params->char_count; i++) {
    printf("%c\n", params->character);
  }

  return NULL; // Nothing to return
}

/**
* Initializes char_print parameters.
*/
void init_params(char_print_params_t params[]) {
  params[0].character = 'A';
  params[0].char_count = 5;

  params[1].character = 'Z';
  params[1].char_count = 6;
}

/**
* Creates threads.
*/
void create_threads(pthread_t threads[], int thread_count,
                    char_print_params_t params[]) {
  int i;
  for (i = 0; i < THREAD_COUNT; i++) {
    create_default_thread(&threads[i], char_print, &params[i]);
  }
}

/**
* Waits for threads, ignoring their return value.
*/
void wait_for_threads(pthread_t threads[], int thread_count) {
  int i;
  for (i = 0; i < thread_count; i++) {
    pthread_join(threads[i], NULL);
  }
}

int main(void) {
  // Thread ID array
  pthread_t threads[THREAD_COUNT];

  // Array of parameters for char_print
  char_print_params_t params[THREAD_COUNT];
  init_params(params);

  create_threads(threads, THREAD_COUNT, params);

  wait_for_threads(threads, THREAD_COUNT);

  return 0;
}
