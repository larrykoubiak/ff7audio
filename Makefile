CC=gcc
CFLAGS = -ansi -pedantic -Wall -c

all: ff7audio ff7audio_d

ff7audio: protracker.o main.o
	$(CC) protracker.o main.o -o ff7audio

protracker.o: protracker.c protracker.h
	$(CC) $(CFLAGS) protracker.c

main.o: main.c main.h
	$(CC) $(CFLAGS) main.c

debug: ff7audio_d

ff7audio_d: protracker_d.o main_d.o
	$(CC) protracker_d.o main_d.o -o ff7audio_d

protracker_d.o: protracker.c protracker.h
	$(CC) $(CFLAGS) -g protracker.c -o protracker_d.o

main_d.o: main.c main.h
	$(CC) $(CFLAGS) -g main.c -o main_d.o
	
clean:
	rm -f *o *exe stdout.txt stderr.txt *~
