/**
Betriebssysteme Blatt 8, Aufgabe 8-2

Datei: add2.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 15.06.2015

Aufgabe:
Unix-Filter, der zwei Integer-Zahlen liest, sie addiert und das Ergebnis
ausgibt.

Nutzungshinweise:
Aufruf ohne Argumente. Liest zwei, nur durch ein '+' getrennte, Ganzzahlen aus
stdin ein.

Verwendete Systemfunktionen:
  char *fgets(char *puffer, int n, FILE *datei) :
  Liest bis zu n-1 Zeichen aus datei schreibt diese in puffer. Das Lesen wird
  durch einen Newline character, End-Of-File oder einen Error gestoppt. Wurden
  Zeichen gelesen und ist kein Error aufgetreten, wird ein '\0' angehangen.

  Quellen: OS X 10.10 man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdlib.h> // exit, atoi
#include <stdio.h> // stdin

#include "mystrlib.c" // split_str

/** Maximum stdin input length */
#define MAX_INPUT_LENGTH 100

/**
 * Parses the input string, to which `input_ptr` points to, converts the results
 * into integers and writes them into an array to which `parsed_ints_arr`
 * points to.
 */
void parse_input(char *input_ptr, int *parsed_ints_arr) {
  char *split_res[2]; // String splitting results
                      // (an array of pointers to the first char)
  int split_res_count; // Count of string splitting results

  split_str(input_ptr, " ", split_res, &split_res_count);

  // printf("[DEBUG] split_res_count: %i\n", split_res_count);
  // printf("[DEBUG] split_res: %s %s\n", split_res[0], split_res[1]);

  if (split_res_count != 2) {
    perror("ERROR: you have to input two space-seperated integers");
    exit(EXIT_FAILURE);
  }

  parsed_ints_arr[0] = atoi(split_res[0]);
  parsed_ints_arr[1] = atoi(split_res[1]);
}

int main(void) {
  char input_buf[MAX_INPUT_LENGTH]; // Input buffer
  int parsed_ints[2]; // Array of the two entered integers

  // Read from stdin
  fgets(input_buf, MAX_INPUT_LENGTH, stdin);

  parse_input(input_buf, parsed_ints);

  printf("%i\n", parsed_ints[0] + parsed_ints[1]);

  return EXIT_SUCCESS;
}
