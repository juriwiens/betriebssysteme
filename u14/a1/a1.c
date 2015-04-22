/**
* Betriebssysteme Blatt 14
*
* Autor: Juri Wiens
* Erstellt: 21.04.2015
*
* chroot:
* Mittels chroot kann das Wurzelverzeichnis für das Programm geändert werden.
* Ein Zugriff auf das darüber befindliche Dateisystem ist dann nicht mehr ohne
* weiteres möglich. Dies kann die Sicherheit eines Programms erhöhen, da
* Einbrecher nur auf den eingeschränkten Teil des Dateisystems hätten.
*
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(void) {
  void *handle;
  double (*sin)(double); // Pointer to sin()
  double x = 3.1415;

  // Change root directory
  int result = chroot("~/tmp");
  if (result == 0) {
    printf("Changed root successfully.\n");
  }

  // Change working directory to new root folder
  chdir("/");

  // Open dynamic library
  if (!(handle = dlopen("libm.so", RTLD_LAZY))) {
    fprintf(stderr, "%s\n", dlerror());
    return 1;
  }

  // Get pointer to sin
  if (!(sin = dlsym(handle, "sin"))) {
    fprintf(stderr, "%s\n", dlerror());
    return 1;
  }

  printf("sin(%f) = %f\n", x, (*sin)(x));

  // Close dynamic library
  dlclose(handle);

  return 0;
}
