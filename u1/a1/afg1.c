/**
* Betriebssysteme Übung 1 Aufgabe 1
*
* Autor: Juri Wiens
* Erstellt: 21.04.2015
*
* Aufgabenstellung:
* Bei einem ersten Aufruf des Programms soll die Ausgabe auf dem Bildschirm
* erscheinen. Rufen Sie das Programm sodann nochmals auf, wobei Sie die Ausgabe
* in eine Datei umlenken. Gibt es Unterschiede in der Ausgabe? Falls ja, warum?
*
* Antwort:
* Die Ausgabe unterscheid sich darin, dass "before fork" bei der Umlenkung in
* eine Datei doppelt ausgegeben wird, jeweils vor der letzen "pid ..." Ausgabe.
* Die Ursache hierfür liegt in der unterschiedlichen Pufferung. Aus
* Performancegründen wird stdout bei der Umleitung in eine Datei voll gepuffert.
* Erzeugt man nun einen Kindprozess, so wird dieser Puffer ebenfalls mitkopiert
* und somit doppelt ausgegeben.
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int glob = 6;
char buf[] = "a write to stdout\n";

int main(void) {
    int   var;
    pid_t pid;

    var = 88;

    if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
        perror("write error");

    printf("before fork\n");

    if ( (pid = fork()) < 0) {
        perror("fork error");
    } else if (pid == 0) {
        glob++;
        var++;
    } else {
        sleep(2);
    }

    printf("pid = %d, glob =%d, var = %d\n", getpid(), glob, var);

    exit(0);
}
