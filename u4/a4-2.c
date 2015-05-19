/**
Betriebssysteme Blatt 4, Aufgabe 4-2

Datei: a4-2.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 18.05.2015

Aufgabe:
Multi-threaded Serverprogramm, welches per Zufallsgenerator Client-Anforderungen
simuliert und pro Anforderung einen Thread erzeugt, welcher sich meldet und
terminiert.

Gewählte Umsetzung:
Ein Serverprogramm, das ein int als Requests entgegennimmt und mit dem doppelten
Wert beantwortet. Außerdem wird ein zusätzlicher Thread erzeugt, der zufällige
Requests simuliert. Nutzt pthread_detach um die Nutzung von pthread_join und das
dadurch verbundene blockierende Warten auf die Terminierung der Threads zu
umgehen.

Nutzungshinweise:
Aufruf ohne Argumente.

Verwendete Systemfunktionen:
  int pthread_detach(pthread_t thread) :
  Markiert einen Thread als unabhängig, sodass nach dessen Terminierung (ohne
  einen Aufruf von pthread_join) alle damit verbundenen Ressourcen wieder
  freigegeben werden. Gibt im Erfolgsfall 0, sonst eine Fehlernummer zurück.

  pthread_t pthread_self(void) :
  Gibt die Thread ID des aufrufenden Threads zurück.

  Quellen: man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdlib.h> // rand, srand
#include <stdio.h> // printf, perror
#include <pthread.h> // pthread_create, pthread_join, pthread_self, pthread_detach
#include <stdbool.h> // bool data type
#include <unistd.h> // sleep

// Request and response message types.
typedef int request_message_t;
typedef int response_message_t;

/**
* Request type.
*/
typedef struct request {
  request_message_t *msg_ptr;
  response_message_t resp; // Response message has to be set here
} request_t;

/**
* Creates thread with default attributes and handles errors.
*/
void create_default_thread(pthread_t *id_ptr, void *(*func_ptr)(void *),
                          void *arg_ptr, bool detached) {
  int res;

  res = pthread_create(id_ptr, NULL, func_ptr, arg_ptr);
  if (res != 0) {
    perror("ERROR: pthread_create failed\n");
  }

  //Detach thread, if flag is set
  if (detached == true) {
    res = pthread_detach(*id_ptr);
    if (res != 0) {
      perror("ERROR: pthread_detach failed\n");
    }
  }
}

/**
* Handles the request.
* Represents main program logic which would be defined by a framework user.
*/
void handle_request(request_t *req_ptr) {
  sleep(3);
  req_ptr->resp = *(req_ptr->msg_ptr) * 2; // Double the request message int.
}

/**
* Processes threads, triggered by client requests.
*/
void *process_request_thread(void *req_msg_voidptr) {
  pthread_t thread_id = pthread_self();

  // Cast req_msg_voidptr back to it's proper type
  request_message_t *req_msg_ptr = (request_message_t *) req_msg_voidptr;

  printf("[thread#%u] Started with req: %d\n", (unsigned int) thread_id,
      *req_msg_ptr);

  // Construct request struct
  request_t *req_ptr = malloc(sizeof(*req_ptr));
  req_ptr->msg_ptr = req_msg_ptr;

  handle_request(req_ptr); // Call handler function

  // Free memory
  free(req_msg_voidptr);
  free(req_ptr);

  printf("[thread#%u] Exit with resp: %d\n", (unsigned int) thread_id,
      req_ptr->resp);
  pthread_exit(NULL); // Exit thread without a return value
}

/**
* Processes incoming requests by starting a handler function in a new
* detached thread.
* Represents a function that a server framework would provide.
*
* The request-message arg is somehow already validated and casted to the proper
* type.
*/
void process_request(request_message_t req_msg) {
  pthread_t thread_id;

  // Allocate memory for req_msg_ptr in the heap to make it visible for threads
  request_message_t *req_msg_ptr = malloc(sizeof(*req_msg_ptr));

  if (req_msg_ptr != NULL) {
    *req_msg_ptr = req_msg; // Copy req_msg to heap

    // Create thread
    create_default_thread(&thread_id, process_request_thread,
        (void *) req_msg_ptr, false);
  } else {
    perror("ERROR: malloc failed");
  }
}

/**
* Generates an endless count of random requests in order to simulate them.
*/
void simulate_requests() {
  request_message_t req_msg;

  srand(time(NULL)); // Initialize random number generator

  while(true) {
    req_msg = rand() % 100;
    printf("Generated request: %d\n", req_msg);
    process_request(req_msg);
    sleep(1); // Sleep for a second to slow down a little bit
  }
}

int main(void) {
  simulate_requests();
}
