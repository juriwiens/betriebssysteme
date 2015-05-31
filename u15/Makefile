# Makefile für Blatt 15

mmu: mmu.c
	$(CC) -o bin/$@ $<

run:
	bin/mmu

# Test-run which should result in physical address 6330
testrun:
	bin/mmu 53946
