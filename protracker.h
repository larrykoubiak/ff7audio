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
	unsigned char sample_idx;
	unsigned short period;
	unsigned short effect;
} ProtrackerNote;

typedef struct _ProtrackerPattern {
	ProtrackerNote notes[4][64];
} ProtrackerPattern;

typedef struct _ProtrackerModule {
	char songname[20];
	ProtrackerSample *samples[31];
	unsigned char songlength;
	unsigned char reset;
	char sequence[128];
	char type[5];
	char nbPatterns;
	ProtrackerPattern *patterns;
} ProtrackerModule;

int ProtrackerGetPatternCount(ProtrackerModule *mod);

char* ProtrackerGetNote(unsigned short period);

#endif