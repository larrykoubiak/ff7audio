#ifndef __PROTRACKER_H__
#define __PROTRACKER_H__

typedef struct {
	char songname[20];
	ProtrackerSample samples*;
} ProtrackerModule;

typedef struct {
	char name[22];
	short length;
	char finetune;
	char volume;
	short repeat;
	short replen;
} ProtrackerSample;
#endif