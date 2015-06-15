# Makefile für Blatt 8

all: pipehello add2 a8-2

pipehello: pipehello.c
	$(CC) -o bin/$@ $<

add2: add2.c
	$(CC) -o bin/$@ $<

a8-2: a8-2.c add2
	$(CC) -o bin/$@ $<
