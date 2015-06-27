/**
Betriebssysteme Blatt 11, Aufgabe 11-1

Datei: msgsnd.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 25.06.2015

Aufgabe:
Schreiben Sie ein C-Programm msgsnd, das eine gegebene Nachricht an eine
bestimmte Message Queue mit Hilfe des System Call msgsnd() sendet. Die Nachricht
hat einen Typ, der durch eine positive ganze Zahl gekennzeichnet ist. Die
Message Queue wird durch einen Key, der eine positive ganze Zahl ist,
identifiziert. Das Programm verlangt vom Betriebssystem die Message Queue.
Falls sie nicht existiert, soll das Betriebsystem sie erzeugen.

Nutzungshinweise:
msgsnd <key> <type> ”text of message”

Verwendete Systemfunktionen:
  int msgget(key_t key, int msgflg) :
  Gibt bei Erfolg die Message Queue ID (unikaler integer Wert > 0) zurück,
  welche `key` zugeordnet ist. Eine neue Message Queue wird erzeugt, falls
  `key` entweder gleich IPC_PRIVATE ist oder diesem noch keine Message Queue
  zugeordnet ist und zusätzlich das IPC_CREAT Flag in `msgflg` gesetzt wurde.
  Über `msgflg` werden auch die Zugriffsrechte auch die Queue gesetzt und durch
  ein bitweises oder mit den Flags verknüpft. Gibt im Fehlerfall -1 zurück.

  int msgsnd(int msqid, const void *msgp, size_t	msgsz, int msgflg) :
  Sendet eine Nachricht an eine Message Queue mit der ID `msqid`. `msgp` zeigt
  dabei auf ein Struct, welches den Nachrichten-Typ als long und den
  Nachrichten-Text als char-Array beinhaltet. `msgsz` gibt die Länge des
  Nachrichten-Textes an. Mittels `msgflg` können Flags gesetzt werden. Gibt bei
  Erfolg 0, sonst -1 zurück.

  Quellen: FreeBSD 10.1 man pages, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> // msgget, msgsnd
#include <sys/ipc.h> // msgget, msgsnd
#include <sys/msg.h> // msgget, msgsnd
#include <string.h> // strncpy

/** Maximum message length */
#define MSG_LENGTH 100

/** Message Queue permissions */
#define MSG_QUEUE_PERM 0600

/** Message struct */
struct {
  long type;
  char text[MSG_LENGTH];
} msg;

int main(int argc, char **argv) {
  key_t msg_queue_key;
  int msg_queue_id;

  // Parse args
  if (argc == 4) {
    msg_queue_key = atoi(argv[1]);
    msg.type = atoi(argv[2]);
    strncpy(msg.text, argv[3], MSG_LENGTH); // Copy message text arg to struct
  } else {
    printf("Usage: msgsnd <key> <type> ”text of message”\n");
    return EXIT_FAILURE;
  }

  // Get Message Queue ID for given key and create it, if it doesn't exist
  msg_queue_id = msgget(msg_queue_key, MSG_QUEUE_PERM | IPC_CREAT);
  if (msg_queue_id == -1) {
    perror("Getting message queue failed");
    return EXIT_FAILURE;
  }

  // Send message to queue
  if (msgsnd(msg_queue_id, &msg, MSG_LENGTH, 0) != 0) {
    perror("Sending message failed");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
