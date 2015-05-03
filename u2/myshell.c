/**
Betriebssysteme Blatt 2

Datei: myshell.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 02.05.2015

Aufgabe:
Schreiben Sie bitte in C einen kleinen Kommandointerpreter myshell, der 5 Unix
Kommandos Ihrer Wahl in einem Menü zur Auswahl bereitstellt.

Verwendete Systemfunktionen:
  char *fgets(char *puffer, int n, FILE *datei) :
  Liest bis zu n-1 Zeichen aus datei schreibt diese in puffer. Das Lesen wird
  durch einen Newline character, End-Of-File oder einen Error gestoppt.

  int strcmp(const char *s1, const char *s2) :
  Vergleicht zwei Strings. Sind diese identisch wird 0, ist s1 kleiner als s2
  dann eine Zahl kleiner 0, sonst eine Zahl größer 0 zurückgegeben.

  pid_t fork(void) :
  Erzeugt einen neuen Kind-Prozess, der eine exakte Kopie des ausführenden
  Prozesses ist. Gibt dem Kind-Prozess 0 und dem Eltern-Prozess die Prozess-ID
  des Kind-Prozesses zurück.

  pid_t wait(int *stat_loc) :
  Wartet, bis sich ein Kind-Prozess beendet und gibt dessen Prozess-ID zurück
  bzw. -1 im Fehlerfall. Weiterhin enthält stat_loc Information darüber, wie
  der Kind-Prozess terminiert ist.

  int execlp(const char *file, const char *arg, ...) :
  Ersetzt das laufende Programm im aktuellen Prozess gegen ein anderes, welches
  durch eine Pfadangabe in file definiert ist. Es kann eine beliebig lange Liste
  von Argumenten übergeben werden, wobei das erste Argument per Konvention dem
  Dateinamen des auszuführenden Programms entsprechen sollte und als letztes
  Argument NULL übergeben werden muss.

  void exit(int status) :
  Terminiert den aktuellen Prozess mit status.

  pid_t getpid(void) :
  Gibt die Prozess-ID des aufrufenden Prozesses zurück.

  int atoi(const char *str) :
  Konvertiert einen String in einen Integer Wert.

  Quellen: man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* Options
*/
char *program_options[] = {
  "date", // Option 1
  "who",  // Option 2
  "tty",  // Option 3
  "ps",   // Option 4
  "ls"    // Option 5
};
int program_options_count = sizeof(program_options) / sizeof(char*);

/**
* Processes the child/program process.
*/
void process_child(int *option_ptr) {
  printf("Starting '%s' with PID: %d\n", program_options[*option_ptr], getpid());
  execlp(program_options[*option_ptr], program_options[*option_ptr], NULL);
}

/**
* Processes the parent/myshell process.
*/
void process_parent(void) {
  int child_status;

  // Wait for child process
  wait(&child_status);
  printf("Your selected program has terminated.\n");
}

/**
* Starts the a new process by forking.
*/
void start_process(int *option_ptr) {
  // Fork process
  pid_t pid = fork();

  switch (pid) {
    case -1:
      // Forking went wrong
      perror("ERROR: Forking process failed.\n");
      break;
    case 0:
      // Child process goes here
      process_child(option_ptr);
      break;
    default:
      // Parent process goes here
      process_parent();
  }
}

int main(void) {
  char input[5]; // Input String Buffer
  int option; // Selected option
  bool terminate = false; // Termination flag

  // Print program options
  for (int i = 0; i < program_options_count; i++) {
    printf("%d: %s\n", i+1, program_options[i]);
  }

  // Ask user for selection
  do {
    printf("Please enter a number or exit: ");
    fgets(input, 5, stdin);

    // Convert first char to int and decrement to match array index
    option = input[0] - '0' - 1;

    if (option >= 0 && option < program_options_count) {
      // Case: user entered a valid option number, so start selected process
      start_process(&option);
    } else if (strcmp(input, "exit") == 0) {
      // Case: user entered "exit"
      printf("Got it, you want to exit...\n");
      terminate = true;
    } else {
      // Case: invalid input
      perror("ERROR: Invalid input.\n");
      return EXIT_FAILURE;
    }

  } while (terminate != true);

  printf("Bye!\n");
  return EXIT_SUCCESS;
}
