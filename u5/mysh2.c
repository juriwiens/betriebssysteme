/**
Betriebssysteme Blatt 5, Aufgabe 5-1

Datei: mysh2.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 24.05.2015

Aufgabe:
Eine einfache Shell, die das einfache Umlenken der Standardein- und ausgabe
beherrscht.

Weitere Aufgabe:
Erläutern Sie das Kommando cpio -ivt
Lösung:
cpio kopiert Dateien zwischen Archiven und Ordnern. Das -it flag bewirkt, dass
ein Archiv von stdin gelesen und dessen Inhalte nach stout gelistet anstatt
kopiert werden. Das -v flag modifiziert das Verhalten noch dahingehend, dass
detailliertere Informationen zu den Inhalten ausgegeben werden.

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

  int open(const char *path, int oflag, ...) :
  Öffnet die Datei unter path zum lesen und/oder schreiben (abhängig von oflag)
  und gibt einen file descriptor bei Erfolg, sonst -1 zurück. Mehrere oflags
  werden mit bitweisem oder (|) verknüpft. Wird das O_CREAT flag verwendet, um
  die Datei bei Nicht-Existenz anzulegen, wird zusätzlich ein dritter Parameter
  vom Typ mode_t erwartet, welcher die Zugriffsrechte definiert.

  int close(int fildes) :
  Schließt den file descriptor fildes. Gibt bei Erfolg 0, sonst -1 zurück.

  char *strerror(int errnum) :
  Gibt zur system error message mit der Nummer errnum einen human-readable
  string zurück.

  Quellen: man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdlib.h> // exit
#include <stdio.h> // printf, fgets, perror, fprintf
#include <string.h> // strsep, strpbrk, strcpy
#include <stdbool.h> // bool type
#include <unistd.h> // fork, close, execlp
#include <fcntl.h> // open
#include <errno.h> // errno

#include "mystrlib.c" // split_str, trim

const int MAX_INPUT_LENGTH = 256;
const int MAX_ARGS_COUNT = 100;

/**
* Struct that includes all information needed to create a new child
* process with stdin and stdout redirections.
*/
typedef struct {
  char *program;        // Parsed program name
  char *program_args[MAX_ARGS_COUNT]; // Parsed program args
  int program_args_count;
  char *stdin_src;      // Parsed standard input source
  char *stdout_target;  // Parsed standard output target
} process_options_t;

/**
* Prompts user and saves input.
*/
void prompt_user(char *input) {
  printf("mysh2: ");
  fgets(input, 256, stdin);
}

/**
* Parses the input string for stdout and stdin redirect characters.
* Returns true is parsing was successful.
*/
bool parse_input(char *input, process_options_t *opts_ptr) {
  char *program_with_args = NULL; // Program command with arguments

  // Reset parsed results
  opts_ptr->program = NULL;
  opts_ptr->stdin_src = NULL;
  opts_ptr->stdout_target = NULL;

  // Search for symbols in input
  char *stdin_symbol_ptr = strpbrk(input, "<");
  char *stdout_symbol_ptr = strpbrk(input, ">");

  // Case 1: No redirections at all
  if (stdin_symbol_ptr == NULL && stdout_symbol_ptr == NULL) {
    program_with_args = trim(input);
  } else // line breaks after else for better readability

  // Case 2: stdin redirection only
  if (stdin_symbol_ptr != NULL && stdout_symbol_ptr == NULL) {
    // Seperate the symbol, so input gets divided into a left and a right side,
    // where input continues as a pointer to the first char of the right side.
    program_with_args = trim(strsep(&input, "<")); // The left side
    opts_ptr->stdin_src = trim(input); // The right side
  } else

  // Case 3: stdout redirection only
  if (stdout_symbol_ptr != NULL && stdin_symbol_ptr == NULL) {
    // Same procedure like in case 2 (see above).
    program_with_args = trim(strsep(&input, ">"));
    opts_ptr->stdout_target = trim(input);
  } else

  // Case 4: stdin and stdout redirection combined
  if (stdin_symbol_ptr != NULL && stdout_symbol_ptr != NULL) {
    program_with_args = trim(strsep(&input, "<"));
    opts_ptr->stdin_src = trim(strsep(&input, ">"));
    opts_ptr->stdout_target = trim(input);
  }

  // Split programm_with_args into program name and args
  split_str(program_with_args, " ", opts_ptr->program_args, &opts_ptr->program_args_count);

  // Add NULL as last argument (because of exec function)
  opts_ptr->program_args[opts_ptr->program_args_count] = NULL;

  // First part should be the program name
  opts_ptr->program = opts_ptr->program_args[0];

  // Finally check for emptiness
  if (opts_ptr->stdin_src != NULL && *opts_ptr->stdin_src == '\0') {
    perror("Bad command: expected a stdin source!\n");
    return false;
  }
  if (opts_ptr->stdout_target != NULL && *opts_ptr->stdout_target == '\0') {
    perror("Bad command: expected a stdout target!\n");
    return false;
  }

  // Some debugging prints
  printf("[DEBUG] program: '%s'\n", opts_ptr->program);
  for (int i = 0; i <= opts_ptr->program_args_count; i++) {
    printf("[DEBUG] program_args[%d]: '%s'\n", i, opts_ptr->program_args[i]);
  }
  printf("[DEBUG] program_args_count: %d\n", opts_ptr->program_args_count);
  printf("[DEBUG] stdin_src: '%s'\n", opts_ptr->stdin_src);
  printf("[DEBUG] stdout_target: '%s'\n", opts_ptr->stdout_target);

  return true;
}

/**
* Starts the a new process by forking and continues the child
* process by calling the function behind process_child_ptr with given args.
* Returns the child pid to the parent process.
*/
pid_t create_child_process(void (*process_child_ptr)(), void *args) {
  // Fork process
  pid_t pid = fork();

  switch (pid) {
    case -1:
      // Forking went wrong
      perror("ERROR: Forking process failed\n");
      exit(EXIT_FAILURE);
      break;
    case 0:
      // Child process goes here
      (*process_child_ptr)(args);
      exit(EXIT_SUCCESS);
      break;
  }

  // Parent process goes here
  return pid;
}

/**
* Starts a program with stdin and stdout redirection options.
*/
void exec_program(process_options_t *opts_ptr) {
  int stdin_fd, stdout_fd; // Stdin and stdout file descriptors

  if (opts_ptr->stdin_src != NULL) {
    // Open stdin source file with read-only flag
    stdin_fd = open(opts_ptr->stdin_src, O_RDONLY);
    if (stdin_fd == -1) {
      fprintf(stderr, "ERROR: Opening STDIN source file '%s' failed: %s\n",
          opts_ptr->stdin_src, strerror(errno));
    }

    // Duplicate file descriptor
    if (dup2(stdin_fd, STDIN_FILENO) == -1) {
      perror("ERROR: Duplicating STDIN file descriptor failed\n");
    };

    // Close file descriptor
    if (close(stdin_fd) == -1) {
      perror("ERROR: Closing STDIN file descriptor failed\n");
    }
  }

  if (opts_ptr->stdout_target != NULL) {
    // Open stdout target file with write-only and create on non-existence flag
    stdout_fd = open(opts_ptr->stdout_target, O_WRONLY | O_CREAT, 0600);
    if (stdout_fd == -1) {
      fprintf(stderr, "ERROR: Opening STDOUT target file '%s' failed: %s\n",
          opts_ptr->stdout_target, strerror(errno));
      exit(EXIT_FAILURE);
    }

    // Duplicate file descriptor
    if (dup2(stdout_fd, STDOUT_FILENO) == -1) {
      perror("ERROR: Duplicating STDOUT file descriptor failed\n");
      exit(EXIT_FAILURE);
    };

    // Close file descriptor
    if (close(stdout_fd) == -1) {
      perror("ERROR: Closing STDIN file descriptor failed\n");
      exit(EXIT_FAILURE);
    }
  }

  // Execute program
  execvp(opts_ptr->program, opts_ptr->program_args);
}

int main(void) {
  char input[MAX_INPUT_LENGTH]; // Input string buffer
  bool terminate = false; // Termination flag
  process_options_t process_opts;

  do {
    prompt_user(input);

    if (strcmp(trim(input), "exit") == 0) { // Look for exit
      terminate = true;
    } else if (parse_input(input, &process_opts) == true) { // Parse input
      create_child_process(exec_program, &process_opts);
      wait(NULL); // Wait for child process
    }
  } while (terminate != true);

  printf("Bye!\n");
  return EXIT_SUCCESS;
}
