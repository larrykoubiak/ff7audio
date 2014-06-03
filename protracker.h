#ifndef __PROTRACKER_H__
#define __PROTRACKER_H__

typedef struct _ProtrackerSample {
	char name[22];
	short length;
	char finetune;
	char volume;
	short repeat;
	short replen;
} ProtrackerSample;

typedef struct _ProtrackerModule {
	char songname[20];
	ProtrackerSample *samples;
} ProtrackerModule;

#endif