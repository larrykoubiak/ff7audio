#ifndef __MAIN_H__
#define __MAIN_H__

#define SAMPLING_FREQ  	48000
#define REVERB_BUF_LEN 	4800
#define OVERSAMPLE		2
#define NUM_CHANNELS	2
#define BUFFER_SAMPLES	16384

#include "protracker.h"

static unsigned long reverb_len;

void load_module(char* filename, ProtrackerModule* mod);

void free_module(ProtrackerModule *mod);

void print_module(ProtrackerModule *mod);

unsigned short swap16(short val) {
	return (val >> 8) | (val <<8);
}
#endif
