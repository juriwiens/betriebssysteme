# Makefile für Blatt 5

mysh2: mysh2.c
	$(CC) -o bin/$@ $<

run:
	bin/mysh2

test: mystrlib_test.c
	$(CC) -o bin/mystrlib_test $<
	bin/mystrlib_test
