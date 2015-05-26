/**
 * @file    mystrlib.c
 * @brief   Juri's personal library for all kind of string related functions.
 * @author  Juri Wiens
 * @version 0.1.0
 */

#include <string.h> // strsep
#include <ctype.h> // isspace

/**
 * Splits a string by replacing all occurences of any character in delim with
 * '\0' (so it mutates the string, maybe you want to make a copy before use!).
 * Writes all substrings into the provided result array and their count into
 * *result_count_ptr.
 */
void split_str(char *str, const char *delim, char **result,
                  int *result_count_ptr) {
  // Points to the beginning of the next substring to process
  char **next_str_ptr = &str;

  // Seperated substring
  char *seperated_str;

  // Internal result counter
  int i = 0;

  // While strsep returns seperated substrings...
  while ((seperated_str = strsep(next_str_ptr, " ")) != NULL) {
    result[i] = seperated_str; // ... write them to the result array
    i++;
  }

  *result_count_ptr = i; // Write result count
}

/**
* Trims all leading and trailing white-spaces from str.
*/
char *trim(char *str) {
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == '\0')  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = '\0';

  return str;
}
