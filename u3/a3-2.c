/**
Betriebssysteme Blatt 3

Datei: a3-2.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 11.05.2015

Aufgabe:
- C-Programm, das einen Zombie-Prozess erzeugt.
- Zeigen, dass ein Zombie-Prozess vorliegt.

Lösung:
- a3-2 starten und PID des Kind-Prozesses merken
- Neuen Terminal öffnen
- $ ps <kind-pid>
- Wird als STAT Z+ angezeigt, so handelt es sich um einen Zombie-Prozess
  (unter Mac OS X)
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

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

void process_parent(pid_t child_pid) {
  printf("Created child process with PID: %d\n", child_pid);

  // Sleep for 90 seconds
  sleep(90);
}

void process_child() {
  printf("I can't stay long...\n");
  exit(EXIT_SUCCESS);
}

int main(void) {
  create_child_process(&process_parent, &process_child);

  return 0;
}
