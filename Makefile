CC=gcc
CFLAGS = -ansi -pedantic -Wall

all: ff7audio

clean:
	rm -f *o ff7audio ff7audio.exe stdout.txt stderr.txt *~

ff7audio: main.o
	$(CC) $(CFLAGS) main.o -o ff7audio

main.o: main.c
	$(CC) $(CFLAGS) -c main.c