# Makefile für Blatt 11

all: msgsnd msgrcv

msgsnd: msgsnd.c bin_dir
	$(CC) -o bin/$@ $<

msgrcv: msgrcv.c bin_dir
	$(CC) -o bin/$@ $<

bin_dir:
	[ -d bin ] || mkdir bin

test: msgsnd msgrcv
	# Send messages
	bin/msgsnd 100 4 "Typ4"
	bin/msgsnd 100 8 "Typ8"
	bin/msgsnd 100 9 "Typ9"
	bin/msgsnd 100 7 "Typ7"
	ipcs -qo # Show Message Queue status
	# Receive some of the messages
	bin/msgrcv 100 8
	bin/msgrcv 100 4
	bin/msgrcv 100 7
	ipcs -qo # Show Message Queue status
