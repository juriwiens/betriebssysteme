/**
Betriebssysteme Blatt 17

Datei: mylib.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Erstellt: 27.04.2015

Aufgabe 1:
C-Modul erstellen mit einer globalen und einer lokalen Funktion und daraus eine
Shared Object Datei kompilieren.

Lösung:
  Die Kompilierung erfolgte in zwei Schritten:
  1. Kompilieren der Object Datei: $ gcc -fPIC -Wall -g -c mylib.c
    Dabei sorgt die Option -fPIC dafür, dass positionsunabhängiger Code erzeugt
    wird.
  2. Linken der Objektdatei: $ gcc -ggdb3 -shared -Wl,-soname,libhello.so.1 –o \
    libhello.so mylib.o -lc
*/

#include "mylib.h"

char *say_hello(void) {
  return "Hello";
}

static char *say_hello_private(void) {
  return "Hello (private)";
}
