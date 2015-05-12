/**
Betriebssysteme Blatt 3

Datei: a3-3.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 12.05.2015

Aufgabe:
Programm schreiben, das einen Kindprozess erstellt und asynchrom zu diesem
weiterläuft. Terminiert das Kind, soll der Elternprozess daraufhin per
Signalhandler reagieren, indem er den Exit Status in eine globale Variable
speichert und diesen ordnungsgemäß "entsorgt".

Verwendete Systemfunktionen:
  int sigaction (int sig_nr, const struct sigaction *handler_neu,
      struct sigaction *handler_alt) :
  Registriert und/oder erfragt einen Signalhandler zum Signal mit der Nummer
  sig_nr, welcher durch folgende Structs definiert wird/ist:

  struct sigaction {
    union     __sigaction_u __sigaction_u; //
    sigset_t  sa_mask; // Signal Maske
    int       sa_flags; // Signal Optionen
  };

  union __sigaction_u {
    void (*__sa_handler)(int);
    void (*__sa_sigaction)(int, struct __siginfo *, void *);
  };

  Gibt im Erfolgsfall 0 und im Fehlerfall -1 zurück.

  Quellen: man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/**
* Sigaction struct for SIGCHLD handler.
*/
struct sigaction action;

/**
* Time in seconds for the parent process to sleep.
*/
const int parent_sleeping_time = 10;

/**
* Time in seconds for the child process to sleep.
*/
const int child_sleeping_time = 3;


/**
* The child's exit status.
*/
int child_process_status;

/**
* Waits for the child process to exit and cleans up its process.
*/
void wait_for_child_process() {
  pid_t child_process_pid;

  child_process_pid = wait(&child_process_status);
  printf("Child process (PID %d) terminated with status: %d\n",
      child_process_pid, WEXITSTATUS(child_process_status));
}

/**
* Starts the a new process by forking and continues the parent and the child
* process by calling the related functions, which are handed over as function
* pointer parameter.
*/
void create_child_process(void (*process_parent_ptr)(),
    void (*process_child_ptr)()) {
  // Fork process
  pid_t pid = fork();

  switch (pid) {
    case -1:
      // Forking went wrong
      fprintf(stderr, "ERROR: Forking process failed\n");
      exit(EXIT_FAILURE);
      break;
    case 0:
      // Child process goes here
      (*process_child_ptr)();
      break;
    default:
      // Parent process goes here
      (*process_parent_ptr)(pid);
  }
}

/**
* Processes parent process.
*/
void process_parent(pid_t child_pid) {
  int unslept_time;

  printf("Created child process with PID: %d\n", child_pid);

  printf("I try to sleep for %d seconds\n", parent_sleeping_time);

  // Sleep will get interrupted by SIGCHLD, if child wakes up earlier
  unslept_time = sleep(parent_sleeping_time);
  printf("You woke me up, although I had another %d seconds to sleep\n",
      unslept_time);
}

/**
* Processes child process.
*/
void process_child() {
  printf("I am a child process with PID: %d and I am going to sleep for %d seconds\n",
      getpid(), child_sleeping_time);

  sleep(child_sleeping_time);

  exit(EXIT_SUCCESS);
}

/**
* Handles SIGCHLD.
*/
void sigchld_handler(int sig) {
  printf("Received SIGCHLD\n");

  // Cleanup zombie child process
  wait_for_child_process();
}

int main(void) {
  // Add handler function to sigaction struct
  action.sa_handler = sigchld_handler;

  // Register sigaction for SIGCHLD, ignoring the previous action with NULL
  if (sigaction(SIGCHLD, &action, NULL)) {
    perror("sigaction() failed");
    return EXIT_FAILURE;
  }

  create_child_process(&process_parent, &process_child);

  printf("Bye");

  return EXIT_SUCCESS;
}
