CC=gcc
CFLAGS = -ansi -pedantic -Wall -c

all: ff7audio ff7audio_d

ff7audio: sdlplayer.o protracker.o main.o
	$(CC) sdlplayer.o protracker.o main.o -o ff7audio `sdl2-config --libs`
	
sdlplayer.o: sdlplayer.c sdlplayer.h
	$(CC) $(CFLAGS) sdlplayer.c `sdl2-config --cflags`

protracker.o: protracker.c protracker.h
	$(CC) $(CFLAGS) protracker.c

main.o: main.c main.h
	$(CC) $(CFLAGS) main.c `sdl2-config --cflags`

debug: ff7audio_d

ff7audio_d: sdlplayer_d.o protracker_d.o main_d.o
	$(CC) sdlplayer_d.o protracker_d.o main_d.o -o ff7audio_d `sdl2-config --libs`
	
sdlplayer_d.o: sdlplayer.c sdlplayer.h
	$(CC) $(CFLAGS) -g sdlplayer.c -o sdlplayer_d.o `sdl2-config --cflags`

protracker_d.o: protracker.c protracker.h
	$(CC) $(CFLAGS) -g protracker.c -o protracker_d.o

main_d.o: main.c main.h
	$(CC) $(CFLAGS) -g main.c -o main_d.o `sdl2-config --cflags`
	
clean:
	rm -f *o *exe stdout.txt stderr.txt *~
