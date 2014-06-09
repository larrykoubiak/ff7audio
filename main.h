#ifndef __MAIN_H__
#define __MAIN_H__

#include "protracker.h"
#include "sdlplayer.h"

void loadProtrackerMod(char* filename, ProtrackerModule* mod);

void freeProtrackerMod(ProtrackerModule *mod);

void printProtrackerMod(ProtrackerModule *mod);

unsigned short swap16(short val) {
	return (val >> 8) | (val <<8);
}
#endif
