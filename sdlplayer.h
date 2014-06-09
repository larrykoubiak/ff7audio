#ifndef __SDL_PLAYER_H__
#define __SDL_PLAYER_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

#define SAMPLING_FREQ  	48000
#define REVERB_BUF_LEN 	4800
#define NUM_CHANNELS	2
#define OVERSAMPLE		2
#define BUFFER_SAMPLES	16384

void SDLPlayer_AudioCallback(void* userdata,Uint8* stream, int len);

void SDLPlayer_Init(char reverb, long nbsamples);

void SDLPlayer_Play();

#endif