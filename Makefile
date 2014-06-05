CC=gcc
CFLAGS = -ansi -pedantic -Wall -c

all: ff7audio

ff7audio: protracker.o main.o
	$(CC) protracker.o main.o -o ff7audio

protracker.o: protracker.c protracker.h
	$(CC) $(CFLAGS) protracker.c

main.o: main.c main.h
	$(CC) $(CFLAGS) main.c

clean:
	rm -f *o ff7audio ff7audio.exe stdout.txt stderr.txt *~
