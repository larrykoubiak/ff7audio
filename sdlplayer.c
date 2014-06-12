#include "sdlplayer.h"
#include "protracker.h"

static SDL_sem *semaphore;
static long reverb_len,filt_l,filt_r;
static long samples_remaining;
static short mix_buffer[ BUFFER_SAMPLES * NUM_CHANNELS * OVERSAMPLE ];

static void SDLPLayer_DownSample(short *input, short *output, long count) {
	long in_idx, out_idx, out_l, out_r;
	in_idx = out_idx = filt_l = filt_r = 0;
	while( out_idx < count) {
		out_l = filt_l + (input[in_idx++] >> 1);
		out_r = filt_r + (input[in_idx++] >> 1);
		filt_l = input[in_idx++] >> 2;
		filt_r = input[in_idx++] >> 2;
		output[out_idx++] = out_l + filt_l;
		output[out_idx++] = out_r + filt_r;
	}	
}

void SDLPlayer_AudioCallback(void* userdata,Uint8* stream, int len) {
	long count;
	count = len * OVERSAMPLE / 4;
	if(samples_remaining<count) {
		memset(stream,0,len);
		count = samples_remaining;
	}
	memset(mix_buffer,0, count* NUM_CHANNELS * sizeof(short));
	ProtrackerGetAudio(mix_buffer,count);
	SDLPLayer_DownSample(mix_buffer,(short*)stream, count);
	samples_remaining -= count;
	if(samples_remaining <=0) SDL_SemPost(semaphore);
}

void SDLPlayer_Init(char reverb, long nbsamples) {
	SDL_AudioSpec audiospec;
	if(reverb=='y') {
		reverb_len = REVERB_BUF_LEN;
	} else {
		reverb_len = 0;
	}
	samples_remaining = nbsamples;
	memset(&audiospec, 0 , sizeof(SDL_AudioSpec));
	audiospec.freq = SAMPLING_FREQ;
	audiospec.format = AUDIO_S16SYS;
	audiospec.channels = NUM_CHANNELS;
	audiospec.samples = BUFFER_SAMPLES;
	audiospec.callback = SDLPlayer_AudioCallback;
	audiospec.userdata = NULL;
	if(SDL_OpenAudio(&audiospec, NULL)!=0) {
		fprintf(stderr,"Couldn't open audio device: %s\n", SDL_GetError() );
	}
}

void SDLPlayer_Play() {
	SDL_PauseAudio(0);
	semaphore = SDL_CreateSemaphore(0);
	if(SDL_SemWait(semaphore)!= 0) {
		fprintf(stderr,"Couldn't wait for SDL semaphore.\n");
	}
	SDL_CloseAudio();
}