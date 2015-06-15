/**
 * @file    myforklib.c
 * @brief   Juri's personal library for child process forking.
 * @author  Juri Wiens
 * @version 0.1.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
* Starts the a new process by forking and continues the child process by
* calling the function behind child_fn_ptr with given args.
* Returns the child pid to the parent process.
*/
pid_t create_child_process(void (*child_fn_ptr)(), void *args) {
  // Fork process
  pid_t pid = fork();

  switch (pid) {
    case -1:
      // Forking went wrong
      perror("ERROR: Forking failed\n");
      exit(EXIT_FAILURE);
      break;
    case 0:
      // Child process goes here
      (*child_fn_ptr)(args);
      exit(EXIT_SUCCESS);
      break;
  }

  // Parent process goes here
  return pid;
}
