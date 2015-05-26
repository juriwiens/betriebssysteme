/**
 * @file    mystrlib_test.c
 * @brief   Simple tests for mystrlib.
 * @author  Juri Wiens
 * @version 0.1.0
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "mystrlib.c"

void test_split_str() {
  char *str, *to_free;
  char *result[3];
  int result_count;

  str = to_free = strdup("ls -a -l");

  split_str(str, " ", result, &result_count);

  assert(result_count == 3);
  assert(strcmp(result[0],"ls") == 0);
  assert(strcmp(result[1],"-a") == 0);
  assert(strcmp(result[2],"-l") == 0);

  free(to_free);
}

int main(void) {
  test_split_str();

  return EXIT_SUCCESS;
}
