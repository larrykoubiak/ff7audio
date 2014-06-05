#ifndef __PROTRACKER_H__
#define __PROTRACKER_H__

typedef struct _ProtrackerSample {
	char name[22];
	unsigned short length;
	unsigned char finetune;
	unsigned char volume;
	unsigned short repeat;
	unsigned short replen;
} ProtrackerSample;

typedef struct _ProtrackerNote {
	char sample_idx;
	short period;
	short effect;
} ProtrackerNote;

typedef struct _ProtrackerModule {
	char songname[20];
	ProtrackerSample *samples[31];
	unsigned char songlength;
	unsigned char reset;
	char sequence[128];
	char type[4];
} ProtrackerModule;

#endif