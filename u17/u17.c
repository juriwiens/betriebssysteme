/**
Betriebssysteme Blatt 17

Datei: u17.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Erstellt: 27.04.2015

Aufgabe 2:
Mit Hilfe von dlopen() und dlsym() sollen die in der "mylib.c" erstellten
Funktionen dynamisch geladen werden.

Verwendete Systemfunktionen:
  void *dlopen(const char *filename, int flags) :
  Öffnet die dynamische Bibliothek zur Laufzeit und gibt einen Zeiger auf diese
  zurück, welcher im Fehlerfall NULL ist.
  Über flags wird das Verhalten für Symbole definiert, die nicht aufgelöst
  werden können.

  void *dlsym(void *handle, const char *symbol) :
  Gibt einen Zeiger auf ein Symbol zurück bzw. NULL im Fehlerfall.

  const char *dlerror(void) :
  Gibt einen "human-readable" Fehlertext zum zuletzt aufgetreteten Fehler zurück.

  int dlclose(void *handle) :
  Schließt die dynamische Bibliothek. Gibt im Erfolgsfall 0 zurück.

  Quelle: man pages & "C von A bis Z"

Das Nachladen einer lokalen Funktion der Bibliothek schlägt mit Meldung
"undefined symbol" fehl.

Aufgabe 3:
Was sind die Vor- und Nachteile des dynamischen Dazubindens von Shared Objects
in ein Programm?

Dadurch, dass dynamische Bibliotheken bei Verwendung in einem Programm nicht
schon beim Kompilieren an dieses gebunden werden, sind entsprechende Programme
kleiner in der Dateigröße.

Verwenden mehrere Programme eine Bibliothek, so
ergibt aus der Tatsache, dass für alle Programme nur eine Instanz der Bibliothek
in den Hauptspeicher geladen werden muss, ein weiterer Vorteil.

Ein Nachteil ist, dass Shared Libraries beim ersten Laden vollständig geladen
werden müssen. Denn im Gegensatz dazu werden bei einer Kompilierung nur die
Objektdateien geladen, die tatsächlich verwendet werden.

Da unter Umständen mehrere Programme nur eine Instanz der Bibliothek verwenden
ist die Abwärtskompatibilität und damit das Einhalten eines einheitlichen
Versionierungsschemas essentiell. Dies kann als "Nachteil" gesehen werden,
gilt jedoch sowieso als "best practice".
*/

#include <stdio.h>
#include <dlfcn.h>

int main(void) {
  void *handle; // Pointer to dynamic library
  char *(*say_hello_ptr)(); // Pointer to say_hello()
  char *(*say_hello_private_ptr)(); // Pointer to say_hello_private()

  // Open library dynamically using the lazy binding flag
  handle = dlopen("./libhello.so", RTLD_LAZY);
  if (handle == NULL) {
    printf("Fehler bei dlopen(): %s\n", dlerror());
  }

  // Get pointer to say_hello()
  say_hello_ptr = dlsym(handle, "say_hello");
  if (say_hello_ptr == NULL) {
    printf("Fehler bei dlsym() für say_hello: %s\n", dlerror());
  }

  // Try to get pointer to say_hello_private(), but will fail with "Undefined symbol"
  say_hello_private_ptr = dlsym(handle, "say_hello_private");
  if (say_hello_private_ptr == NULL) {
    printf("Fehler bei dlsym() für say_hello_private: %s\n", dlerror());
  }

  // Print say_hello()'s return value
  printf("%s\n", (*say_hello_ptr)());

  // Close dynamic library
  dlclose(handle);

  return 0;
}
