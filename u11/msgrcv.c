/**
Betriebssysteme Blatt 11, Aufgabe 11-1

Datei: msgrcv.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 27.06.2015

Aufgabe:
Schreiben Sie als Gegenstück zu msgsnd ein C-Progamm msgrcv, das aus einer durch
<key> gekennzeichneten Message Queue eine Nachricht vom Typ <type> mit Hilfe des
System Call msgrcv() liest. Das Programm setzt voraus, dass die
Nachrichtenwarteschlange existiert.

Nutzungshinweise:
msgrcv <key> <type>

Verwendete Systemfunktionen:
  int msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg) :
  Empfängt eine Nachricht aus der mit `msgqid` angegebenen Message Queue und
  platziert diese in ein von `msgp` referenziertes Struct. `msgsz` spezifiziert
  die maximale Länge der Nachricht. Mit `msgtyp` wird festgelegt, welcher
  Nachrichten-Typ empfangen werden soll:
  msgtyp == 0 : Typ spielt keine Rolle, erste Nachricht wird empfangen (FIFO).
  msgtyp == N : Erste Nachricht, die den Typ N hat, wird empfangen.
  msgtyp < 0  : Erste Nachricht des kleinsten Typs, der kleiner oder gleich
                dem absoluten Wert von `msgtyp` ist, wird empfangen.
  Das Verhalten, falls keine entsprechende Nachricht vorhanden ist, wird über
  `msgflg` gesteuert: wird das Flag IPC_NOWAIT gesetzt, so wird sofort -1
  zurückgegeben, sonst blockiert die Funktion. Gibt im Erfolgsfall die Anzahl
  der empfangenen Bytes des Nachrichten-Textes zurück, sonst -1.

  Quellen: FreeBSD 10.1 man pages, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> // msgget, msgrcv
#include <sys/ipc.h> // msgget, msgsrcv
#include <sys/msg.h> // msgget, msgrcv

/** Maximum message length */
#define MSG_LENGTH 100

/** Message struct */
struct {
  long type;
  char text[MSG_LENGTH];
} msg;

int main(int argc, char **argv) {
  key_t msg_queue_key;
  int msg_queue_id;

  // Parse args
  if (argc == 3) {
    msg_queue_key = atoi(argv[1]);
    msg.type = atoi(argv[2]);
  } else {
    printf("Usage: msgrcv <key> <type>\n");
    return EXIT_FAILURE;
  }

  // Try to get the Message Queue ID for given key
  msg_queue_id = msgget(msg_queue_key, 0);
  if (msg_queue_id == -1) {
    perror("Getting message queue failed");
    return EXIT_FAILURE;
  }

  // Receive message and don't block, if no matching message is present
  if (msgrcv(msg_queue_id, &msg, MSG_LENGTH, msg.type, IPC_NOWAIT) == -1) {
    perror("Receiving message failed");
    return EXIT_FAILURE;
  }

  printf("Received message: %s\n", msg.text);

  return EXIT_SUCCESS;
}
