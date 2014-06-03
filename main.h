#ifndef __MAIN_H__
#define __MAIN_H__

#define REVERB_BUF_LEN 4800
#include "protracker.h"

static long reverb_len;

void load_module(char *filename);
void print_module(ProtrackerModule *mod);

#endif
