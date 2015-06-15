/**
Betriebssysteme Blatt 8, Aufgabe 8-1

Datei: pipehello.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 15.06.2015

Aufgabe:
Elternprozess soll einen Kind-Prozess erzeugen und diesem über eine Pipe
"Hello World" übermitteln. Letzterer gibt den erhaltenen String nach Stdout aus.

Nutzungshinweise:
Aufruf ohne Argumente.

Verwendete Systemfunktionen:
  int pipe(int fildes[2]) :
  Erzeugt eine Pipe and zwei dazugehörige File Descriptors, jeweils einen für
  das lesende (Index 0) und das schreibende Ende (Index 1) der Pipe. Gibt bei
  Erfolg 0, sonst -1 zurück. Die maximale Anzahl der Daten, die in eine Pipe
  geschickt werden können, sind mit der Konstante PIPE_BUF (limits.h) definiert.

  ssize_t write(int fildes, const void *buf, size_t nbyte) :
  Write versucht `nbyte` Daten von einem Buffer, auf welchen `buf` zeigt, auf
  `fildes` zu schreiben. Gibt bei Erfolg die Anzahl der geschriebenen Bytes,
  sonst -1 zurück.

  ssize_t read(int fildes, void *buf, size_t nbyte) :
  Read versucht `nbyte` Daten von `fildes` in einen Buffer, auf welchen `buf`
  zeigt, einzulesen. Gibt bei Erfolg die Anzahl der geschriebenen Bytes,
  sonst -1 zurück.

  int sprintf(char * restrict str, const char * restrict format, ...) :
  Baut einen String analog zu printf auf Grundlage von `format` und schreibt
  diesen in `str`.

  size_t strlen(const char *s) :
  Gibt die Länge eines Strings ohne Berücksichtigung von '\0' zurück.

  Quellen: OS X 10.10 man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdlib.h> // exit
#include <stdio.h> // sprinft, perror
#include <unistd.h> // pipe, write, read
#include <sys/types.h> // write, read
#include <sys/uio.h> // write, read
#include <limits.h> // PIPE_BUF
#include <string.h> // strlen

#include "myforklib.c" // create_child_process

/** Pipe file descriptors */
int pipe_fds[2];

void child_main(void *args) {
  char read_buf[PIPE_BUF]; // Pipe read buffer
  size_t read_buf_size; // Current pipe read buffer size

  // Close writeable pipe end
  close(pipe_fds[1]);

  // Read from pipe
  read_buf_size = read(pipe_fds[0], read_buf, PIPE_BUF);

  // Write to stdout
  if (write(STDOUT_FILENO, read_buf, read_buf_size) != read_buf_size) {
    perror("ERROR: Writing to stdout failed");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

int main(void) {
  char pipe_buf[PIPE_BUF]; // Pipe buffer
  size_t pipe_buf_size; // Current size of pipe buffer

  // Fill pipe buffer with "Hello World"
  sprintf(pipe_buf, "Hello World");
  pipe_buf_size = strlen(pipe_buf) + 1; // Add one to include '\0'

  // Create pipe
  if ((pipe(pipe_fds)) != 0) {
    perror("ERROR: Creating pipe failed");
    exit(EXIT_FAILURE);
  }

  create_child_process(&child_main, NULL);

  // Close readable pipe end
  close(pipe_fds[0]);

  // Write to pipe
  if ((write(pipe_fds[1], pipe_buf, pipe_buf_size) != pipe_buf_size)) {
    perror("ERROR: Writing to pipe failed");
    exit(EXIT_FAILURE);
  }

  // Wait for child process, ignoring it's status
  wait(NULL);

  return EXIT_SUCCESS;
}
