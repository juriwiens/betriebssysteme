/**
Betriebssysteme Blatt 8, Aufgabe 8-2

Datei: a8-2.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 15.06.2015

Aufgabe:
Programm, das zwei Integer-Zahlen von stdin liest und sie über eine Pipe pipe1
an einen Kind-Prozess gibt, der die Summe beider Zahlen bildet. Über eine
weitere Pipe pipe2 gibt der Kind-Prozess das Ergebnis an den Eltern-Prozess
zurück, der es nach stdout ausgibt. In dem Kind-Prozess soll das Programm add2
laufen.

Nutzungshinweise:
Aufruf ohne Argumente. Liest zwei, nur durch ein Leerzeichen getrennte,
Ganzzahlen aus stdin ein.
*/

#include <stdlib.h> // exit
#include <stdio.h> // sprinft, perror
#include <unistd.h> // pipe, write, read
#include <sys/types.h> // write, read
#include <sys/uio.h> // write, read
#include <limits.h> // PIPE_BUF
#include <string.h> // strlen

#include "myforklib.c" // create_child_process

/** Hardcoded path to add2, because it's not in $PATH. I know, it's ugly :-) */
const char *ADD2_PATH = "/Users/juriwiens/gitdev/betriebssysteme/u8/bin/add2";

/** Pipe file descriptors */
int pipe1_fds[2];
int pipe2_fds[2];

/** The child process' "main function" */
void child_main(void *args) {
  // Close writeable pipe1 end and readable pipe2 end
  close(pipe1_fds[1]);
  close(pipe2_fds[0]);

  // Duplicate the readable pipe1 end to stdin and close it afterwards
  if (dup2(pipe1_fds[0], STDIN_FILENO) != STDIN_FILENO) {
   perror("ERROR: Duplicating pipe1 to stdin failed");
   exit(EXIT_FAILURE);
  }
  close(pipe1_fds[0]);

   // Duplicate the writeable pipe2 end to stdout and close it afterwards
  if (dup2(pipe2_fds[1], STDOUT_FILENO) != STDOUT_FILENO) {
    perror("ERROR: Duplicating pipe2 to stdout failed");
    exit(EXIT_FAILURE);
  }
  close(pipe2_fds[1]);

  execlp(ADD2_PATH, "add2", NULL);

  // if execlp returns, something went wrong
  perror("ERROR: execlp failed");
  exit(EXIT_FAILURE);
}

int main(void) {
  char pipe1_buf[PIPE_BUF], pipe2_buf[PIPE_BUF]; // Pipe buffers
  size_t pipe1_buf_size, pipe2_buf_size; // Current sizes of pipe buffers

  // Read from stdin into pipe1 buffer
  fgets(pipe1_buf, PIPE_BUF, stdin);
  pipe1_buf_size = strlen(pipe1_buf) + 1; // Add one to include '\0'

  // Create pipes
  if (((pipe(pipe1_fds)) || (pipe(pipe2_fds))) != 0) {
    perror("ERROR: Creating pipes failed");
    exit(EXIT_FAILURE);
  }

  create_child_process(&child_main, NULL);

  // Close readable pipe1 end and writeable pipe2 end
  close(pipe1_fds[0]);
  close(pipe2_fds[1]);

  // Write pipe1_buf to pipe1
  if ((write(pipe1_fds[1], pipe1_buf, pipe1_buf_size) != pipe1_buf_size)) {
    perror("ERROR: Writing to pipe1 failed");
    exit(EXIT_FAILURE);
  }

  // Read from pipe2 into pipe2_buf
  pipe2_buf_size = read(pipe2_fds[0], pipe2_buf, PIPE_BUF);

  // Write pipe2_buf to stdout
  if (write(STDOUT_FILENO, pipe2_buf, pipe2_buf_size) != pipe2_buf_size) {
    perror("ERROR: Writing to stdout failed");
    exit(EXIT_FAILURE);
  }

  // Wait for child process, ignoring it's status
  wait(NULL);

  return EXIT_SUCCESS;
}
