/**
Betriebssysteme Blatt 15

Datei: mmu.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 31.05.2015

Aufgabe 1:
Gegeben sei ein System mit einem 16-Bit Adressraum, einer Pagesize von 4096 Bit
und 32KB physikalischem Hauptspeicher.

- Wieviele Einträge hat die Seitentabelle?
  Pagesize 4096 Bit / 8 = 512 Byte
  16 Bit Adressraum => 2^16 mögliche virtuelle Adressen
                    => 2^16 Byte / 512 Byte = 128 Seiten
  => Die Seitentabelle hat 128 Einträge (gleich der Seitenanzahl)

- Wieviele Pages können gleichzeitig im Speicher gehalten werden?
  32 KB Hauptspeicher => (32 KB * 1024) / 512 Byte = 64 Seitenrahmen
  => 64 Pages/Seiten können gleichzeitig im (Haupt-)Speicher gehalten werden

Aufgabe 2:
Eine MMU mit den oben genannten Systemparametern simulieren mit beispielhaftem
Zustand der Seitentabelle. Page faults sollen durch eine Ausgabe kenntlich
gemacht werden.

Weitere Berechnungen hierfür:
  128 Seiten  => erste 7 bit von virtueller Adresse für Seitenadressierung
              => weitere 16-7 = 9 bit Offset

  64 Seitenrahmen => erste 6 bit von physischer Adresse für Seitenrahmen
                  => weitere 9 Bit Offset
                  => Physische Adresse insgesamt 15 bit lang

Nutzungshinweise:
  mmu [-p] virtual_address

  Gibt zur eingebenen virtuellen Adresse die physikalische aus.
  Ein optionales -p flag bewirkt, dass die komplette Seitentabelle ebenfalls
  mit ausgegeben wird.
*/

#include <stdlib.h> // exit
#include <stdio.h> // printf
#include <stdbool.h> // bool data type
#include <string.h> // strcmp

/**
 * Pageframe number type.
 * Because pageframe numbers are only 6 bit long, unsigned short (16 bit) is
 * more than enough.
 */
typedef unsigned short pageframe_no_t;

/**
 * Page number type.
 */
typedef unsigned short page_no_t;

/**
 * Offset type.
 */
typedef unsigned short offset_t;

/**
 * Virtual address type.
 */
typedef unsigned short virtual_address_t;

/**
 * Physical address type.
 */
typedef unsigned short physical_address_t;

/**
 * Pagetable entry struct.
 */
typedef struct {
  bool pa; // Present/Absent bit. True means present.
  pageframe_no_t pageframe_no; // 6 bit pageframe number, so short is enough
} pagetable_entry_t;

/**
 * Pagetable, that consists of 128 entries.
 */
pagetable_entry_t pagetable[128] = {};

/**
 * Bitmask for extracting the offset from a virtual address via bitwise-and.
 * It's the decimal representation of the binary number 0000000111111111
 */
const unsigned short offset_bitmask = 511;

/**
 * Initialises pagetable with some example entries.
 */
void init_pagetable() {
  // Pagetable entry no. 1 points to pageframe no. 0
  pagetable[1].pa = 1;
  pagetable[1].pageframe_no = 0;

  // Pagetable entry no. 10 points to pageframe no. 15
  pagetable[10].pa = 1;
  pagetable[10].pageframe_no = 15;

  // Pagetable entry no. 30 points to pageframe no. 60
  pagetable[30].pa = 1;
  pagetable[30].pageframe_no = 60;

  // Pagetable entry no. 105 points to pageframe no. 12
  pagetable[105].pa = 1;
  pagetable[105].pageframe_no = 12;

  // All other entries are "absent".
}

/**
 * Prints whole pagetable.
 */
void print_pagetable() {
  int i;

  printf("=============\n");
  for (i = 0; i < 128; i++) {
    printf("#%.3u | %d | %.2u\n", i, pagetable[i].pa, pagetable[i].pageframe_no);
  }
  printf("=============\n\n");
}

/**
 * Translates virtual into physical addresses.
 */
physical_address_t translate(virtual_address_t virtual_address) {
  page_no_t page_no;
  pageframe_no_t pageframe_no;
  offset_t offset;
  physical_address_t physical_address;

  // Get page number by shifting the first 9 bits out of the virtual address.
  page_no = virtual_address; // Copy the value first
  page_no>>=9;
  printf("Extracted page_no: %.3u\n", page_no);

  // Get offset by bitmasking the virtual address
  offset = virtual_address & offset_bitmask;
  printf("Extracted offset: %.3u\n", offset);

  if (pagetable[page_no].pa == 1) { // Check P/A bit
    // P/A bit was 1, so lookup pageframe number in pagetable
    pageframe_no = pagetable[page_no].pageframe_no;
    printf("Found pageframe_no: %.2u\n", pageframe_no);

    // Assemble physical address
    physical_address = pageframe_no; // First assign pageframe number...
    physical_address<<=9; // ...then shift it to the left end...
    physical_address += offset; // ...and finally add the offset.

    return physical_address;
  } else {
    // P/A bit was 0, so we detected a page fault
    printf("PAGE FAULT\n");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char **args) {
  physical_address_t physical_address;
  virtual_address_t virtual_address = 0;

  init_pagetable();

  // Parse args
  if (argc == 3 && strcmp(args[1], "-p") == 0) {
    print_pagetable();
    virtual_address = atoi(args[2]);
  } else if (argc == 2) {
    virtual_address = atoi(args[1]);
  } else {
    printf("Usage: mmu [-p] virtual_address\n");
  }

  physical_address = translate(virtual_address);
  printf("Resulting physical address: %.5u\n", physical_address);

  return EXIT_SUCCESS;
}
