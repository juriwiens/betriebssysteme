/**
Betriebssysteme Blatt 3

Datei: a3-1.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 10.05.2015

Aufgabe:
Schreiben Sie bitte in C einen kleinen Kommandointerpreter myshell, der 5 Unix
Kommandos Ihrer Wahl in einem Menü zur Auswahl bereitstellt.

Nutzungshinweise:
Aufruf mit optionalem --endless Flag führt dazu, dass theoretisch unendliche
viele Kindprozesse erzeugt werden.

Verwendete Systemfunktionen:
  unsigned int sleep(unsigned int seconds) :
  Setzt die Ausführung des aktuellen Programms aus, bis entweder seconds
  Sekunden vergangen sind oder ein bestimmtes signal empfangen wurde. Das
  Aussetzen kann eventuell länger dauern als definiert. Gibt 0 zurück, falls die
  angegebene Sekundenanzahl verstrichen ist, oder die verbliebenen Sekunden,
  falls die Ausführung (aufgrund eines signals) früher fortgesetzt wurde.

  int rand() :
  Gibt eine (Pseudo-)Zufallszahl zurück, die einen Wert zwischen einschließlich
  0 und RAND_MAX annehmen kann. Eine wird eine vorherige Initialisierung des
  Generators über void srand(unsigned seed) durch Übergabe eines Zeitstempels
  empfohlen.

  time_t time(time_t *tloc) :
  Gibt die Anzahl der verstrichenen Sekunden seit dem 01.01.1970 00:00 UTC
  zurück. Zusätzlich kann der Wert in *tloc abgelegt werden.

  WEXITSTATUS(status) :
  Makro, welches zu der von wait() geschriebenen Statusinformation, das
  Argument des exit() Aufrufs des Kindprozesses zurückgibt.

  Quellen: man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.

Schwierigkeiten:
Der einmalige Aufruf von srand() im main() des Elternprozesses führte dazu,
dass rand() in allen Kindprozessen dieselbe Zufallszahl lieferte. Erst der
individuelle Aufruf von srand() innerhalb jedes Kindprozesses mit
unterschiedlichen seed Parametern führte zu unterschiedlichen Zufallszahlen.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

/**
* Default number of child processes to create.
*/
const int CHILD_PROCESSES = 3;

/**
* Counter for created child processes.
*/
int child_processes_created = 0;

/**
* Counter for terminated child processes.
*/
int child_processes_terminated = 0;

/**
* Flag for producing an endless amount of child processes.
*/
bool endless_mode = false;

/**
* Starts the a new process by forking and continues the parent and the child
* process by calling the related functions, which are handed over as function
* pointer parameter.
*/
void create_child_process(void (*process_parent_ptr)(), void (*process_child_ptr)()) {
  // Fork process
  pid_t pid = fork();

  switch (pid) {
    case -1:
      // Forking went wrong
      fprintf(stderr, "ERROR: Forking process failed for no. %d\n",
          ++child_processes_created);
      exit(EXIT_FAILURE);
      break;
    case 0:
      // Child process goes here
      (*process_child_ptr)();
      break;
    default:
      // Parent process goes here
      (*process_parent_ptr)();
  }
}

void process_parent() {
  child_processes_created++;
  printf("Created child process\n");
}

void process_child() {
  // Initialize random number generator and generate random number mod 10
  srand(time(NULL) + getpid());
  int sleeping_time = rand() % 10;

  printf("I am a child process no. %d with PID: %d and I am going to sleep for %d seconds\n",
      ++child_processes_created, getpid(), sleeping_time);

  // Sleep a random amount of 0 to 10 seconds
  sleep(sleeping_time);

  exit(EXIT_SUCCESS);
}

void wait_for_child_processes() {
  pid_t child_process_pid;
  int child_process_status;

  while(child_processes_terminated < CHILD_PROCESSES) {
    child_process_pid = wait(&child_process_status);
    child_processes_terminated++;
    printf("Child process with PID %d terminated with status %d\n",
        child_process_pid, WEXITSTATUS(child_process_status));
  }
}

int main(int argc, char *argv[]) {
  // Check for endless mode flag
  if (argc == 2 && strcmp(argv[1], "--endless") == 0) {
    endless_mode = true;
  }

  // Create child processes
  while (child_processes_created < CHILD_PROCESSES || endless_mode == true) {
    create_child_process(&process_parent, &process_child);
  }

  wait_for_child_processes();

  printf("All of my children terminated, so me too.\n");
  return EXIT_SUCCESS;
}
