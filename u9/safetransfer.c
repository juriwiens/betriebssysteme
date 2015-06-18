/**
Betriebssysteme Blatt 9, Aufgabe 9-1

Datei: safetransfer.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 18.06.2015

Aufgabe:
Ein Programm welches den sicheren Transfer zwischen 3 Bankkonten simuliert,
indem zwei Threads erzeugt werden. In dem einen thread wird ein Betrag X von
Konto 1 nach Konto 2 transferiert. In dem anderen thread wird ein anderer
Betrag Y von Konto 2 nach Konto 3 transferiert. Der exklusive Zugriff soll über
Mutexes abgesichert werden.

Nutzungshinweise:
Aufruf ohne Argumente.

Verwendete Systemfunktionen:
  int pthread_mutex_lock(pthread_mutex_t *mutex) :
  Sperrt die Variable `mutex`. Sollte diese bereits gesperrt sein, blockiert
  der Thread, bis `mutex` wieder verfügbar ist. Gibt bei Erfolg 0, sonst eine
  Fehlernummer zurück.

  int pthread_mutex_unlock(pthread_mutex_t *mutex) :
  Entsperrt die Variable `mutex`. Gibt bei Erfolg 0, sonst eine Fehlernummer
  zurück.

  Quellen: OS X 10.10 man pages, cppreference.com, sowie "C von A bis Z" und
  "Linux-Unix Programmierung" vom Rheinwerk Verlag.
*/

#include <stdio.h> // perror
#include <pthread.h> // pthread_*

/** Array of account balances, initialized with some arbitrary values */
float account_balances[] = {50.0f, 100.0f, 1000.0f};

/** Count of transfers */
const int transfer_count = 2;

/** Array of threads */
pthread_t threads[transfer_count];

/**
 * Mutex for critical section in process_transfer.
 * Has to be initialized with PTHREAD_MUTEX_INITIALIZER!
 */
pthread_mutex_t process_transfer_mutex = PTHREAD_MUTEX_INITIALIZER;

/** Transfer arguments struct type */
typedef struct {
  int from_acct;
  int to_acct;
  float amount;
} transfer_args_t;

/**
 * Transfers `amount` from `from_acct` to `to_acct`.
 * Returns 0 on success, else an error number.
 */
int process_transfer(int from_acct, int to_acct, float amount) {
  // Lock critical section
  if (pthread_mutex_lock(&process_transfer_mutex) != 0) {
    perror("ERROR: Locking mutex failed");
    return -1;
  }

  printf("Current balances of account #%d and #%d: %.2f | %.2f\n", from_acct,
      to_acct, account_balances[from_acct], account_balances[to_acct]);

  account_balances[from_acct] -= amount; // Charge from_acct
  account_balances[to_acct] += amount; // Credit to_acct

  printf("Transfered %.2f units from account #%d to #%d. New balances: %.2f | %.2f\n",
      amount, from_acct, to_acct, account_balances[from_acct],
      account_balances[to_acct]);

  // Unlock critical section
  if (pthread_mutex_unlock(&process_transfer_mutex) != 0) {
    perror("ERROR: Unlocking mutex failed");
    return -1;
  }

  return 0;
}

/**
 * Gets called by pthread_create. Takes a transfer_args_t argument and calls
 * process_transfer with it.
 */
void *transfer_thread_fn(void *args_ptr) {
  // Cast args_ptr to transfer_t
  transfer_args_t *transfer_args_ptr = (transfer_args_t *) args_ptr;

  if (process_transfer( transfer_args_ptr->from_acct,
                        transfer_args_ptr->to_acct,
                        transfer_args_ptr->amount)) {
    perror("ERROR: Transfer from account 1 to account 2 failed\n");
  }

  pthread_exit(NULL); // Exit thread without a return value
}

/** Waits for threads, ignoring their return value. */
void wait_for_threads(pthread_t threads[], int thread_count) {
  int i;
  for (i = 0; i < thread_count; i++) {
    pthread_join(threads[i], NULL);
  }
}

/** Creates a thread which starts with transfer_thread_fn */
void create_transfer_thread(pthread_t *thread_ptr,
                            transfer_args_t *transfer_args_ptr) {
  if (pthread_create(thread_ptr, NULL, transfer_thread_fn,
      transfer_args_ptr) != 0) {
    perror("ERROR: Creating thread failed");
  }
}

int main(void) {
  // Prepare transfer args with arbitrary values
  transfer_args_t transfer_args[transfer_count] = {
    {0, 1, 20.0f}, // 20.0 money units from account 0 to 1
    {1, 2, 70.0f} // 70.0 money units from account 1 to 2
  };

  // Create threads, which start a transfer with given transfer args
  for (int i = 0; i < transfer_count; i++) {
    create_transfer_thread(&threads[i], &transfer_args[i]);
  }

  wait_for_threads(threads, transfer_count);

  return 0;
}
