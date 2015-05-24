/**
Betriebssysteme Blatt 5, Aufgabe 5-1

Datei: mysh2.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 24.05.2015

Aufgabe:
Eine einfache Shell, die das Umlenken der Standardein- und ausgabe beherrscht.

Nutzungshinweise:
Aufruf ohne Argumente.

Verwendete Systemfunktionen:
int dup2(int fildes, int fildes2) :
  Dupliziert den file descriptor fildes auf fildes2 als Ziel. Ist fildes2
  bereits in Verwendung, wird dieser zuvor geschlossen. Gibt bei Erfolg den
  neuen file descriptor zurück, bei Fehlern -1.

  char *strpbrk(const char *s1, const char *s2) :
  Sucht in s1 nach dem Vorkommen eines Zeichens aus s2 und gibt dessen Zeiger
  zurück, sonst NULL.

  char *strsep(char **stringp, const char *delim) :
  Sucht in stringp nach dem ersten Auftreten irgendeines der in delim
  angegebenen Zeichen und ersetzt es mit '\0'. Die Adresse des darauf folgenden
  char* wird dabei in stringp abgelegt und der Wert von stringp zurückgegeben.

  int isspace(int c) :
  Testet, ob c ein white-space character ist. Gibt 0 zurück falls dem nicht so
  ist, sonst eine Zahl ungleich 0.

  char *strcpy(char *restrict dst, const char *restrict src) :
  Kopiert den src String in dst (inkl. '\0') und gibt dst zurück.

  Quellen: man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdio.h> // printf, fgets
#include <string.h> // strsep, strpbrk, strcpy
#include <ctype.h> // isspace
#include <stdbool.h> // bool type

const int MAX_INPUT_LENGTH = 256;

/**
* Parsed results struct.
*/
struct {
  char *program;        // Parsed program name
  char *stdin_src;      // Parsed standard input source
  char *stdout_target;  // Parsed standard output target
} parsed_res;

/**
* Trims all leading and trailing white-spaces from str.
*/
char *trim(char *str) {
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = '\0';

  return str;
}

/**
* Prompts user and saves input.
*/
void prompt_user(char *input) {
  printf("mysh2: ");
  fgets(input, 256, stdin);
}

/**
* Parses the input string for stdout and stdin redirect characters.
*/
void parse_input(char *input) {
  // Reset parsed results
  parsed_res.program = NULL;
  parsed_res.stdin_src = NULL;
  parsed_res.stdout_target = NULL;

  // Search for symbols in input
  char *stdin_symbol_ptr = strpbrk(input, "<");
  char *stdout_symbol_ptr = strpbrk(input, ">");

  // Case 1: No redirections at all
  if (stdin_symbol_ptr == NULL && stdout_symbol_ptr == NULL) {
    parsed_res.program = trim(input);
  } else // line breaks after else for better readability

  // Case 2: stdin redirection only
  if (stdin_symbol_ptr != NULL && stdout_symbol_ptr == NULL) {
    // Seperate the symbol, so input gets divided into a left and a right side,
    // where input continues as a pointer to the first char of the right side.
    parsed_res.program = trim(strsep(&input, "<")); // The left side
    parsed_res.stdin_src = trim(input); // The right side
  } else

  // Case 3: stdout redirection only
  if (stdout_symbol_ptr != NULL && stdin_symbol_ptr == NULL) {
    // Same procedure like in case 2 (see above).
    parsed_res.program = trim(strsep(&input, ">"));
    parsed_res.stdout_target = trim(input);
  } else

  // Case 4: stdin and stdout redirection combined
  if (stdin_symbol_ptr != NULL && stdout_symbol_ptr != NULL) {
    parsed_res.program = trim(strsep(&input, "<"));
    parsed_res.stdin_src = trim(strsep(&input, ">"));
    parsed_res.stdout_target = trim(input);
  }

  // Finally check for emptiness
  if (parsed_res.stdin_src != NULL && *parsed_res.stdin_src == '\0') {
    fprintf(stderr, "Bad command: expected a stdin source!\n");
    return;
  }
  if (parsed_res.stdout_target != NULL && *parsed_res.stdout_target == '\0') {
    fprintf(stderr, "Bad command: expected a stdout target!\n");
    return;
  }

  printf("[DEBUG] program: '%s'\n", parsed_res.program);
  printf("[DEBUG] stdin_src: '%s'\n", parsed_res.stdin_src);
  printf("[DEBUG] stdout_target: '%s'\n", parsed_res.stdout_target);
}

int main(void) {
  char input[MAX_INPUT_LENGTH]; // Input String Buffer
  bool terminate = false; // Termination flag

  do {
    prompt_user(input);
    parse_input(input);

    if (strcmp(input, "exit") == 0) {
      terminate = true;
    }
  } while (terminate != true);

  printf("Bye!\n");
  return 0;
}
