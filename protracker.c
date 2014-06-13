#include <stdlib.h>
#include <string.h>
#include "protracker.h"

static unsigned short ProtrackerPeriods[] = {
1712,1616,1524,1440,1356,1280,1140,1076,1016,960,906,
856,808,762,720,678,640,604,570,538,508,480,453,
428,404,381,360,339,320,302,285,269,254,240,226,
214,202,190,180,170,160,151,143,135,127,120,113,
107,101,95,90,85,80,75,71,67,63,60,56};

static char* ProtrackerNotes[] = {
"C-0","C#0","D-0","D#0","E-0","F-0","F#0","G-0","G#0","A-0","A#0","B-0",
"C-1","C#1","D-1","D#1","E-1","F-1","F#1","G-1","G#1","A-1","A#1","B-1",
"C-2","C#2","D-2","D#2","E-2","F-2","F#2","G-2","G#2","A-2","A#2","B-2",
"C-3","C#3","D-3","D#3","E-3","F-3","F#3","G-3","G#3","A-3","A#3","B-3",
"C-4","C#4","D-4","D#4","E-4","F-4","F#4","G-4","G#4","A-4","A#4","B-4"};

const char* ProtrackerTypes[21][2] = {
{"M.K.","4"},
{"M!K!","4"},
{"N.T.","4"},
{"FLT4","4"},
{"FLT8","8"},
{"2CHN","2"},
{"4CHN","4"},
{"6CHN","6"},
{"8CHN","8"},
{"10CH","10"},
{"12CH","12"},
{"14CH","14"},
{"16CH","16"},
{"18CH","18"},
{"20CH","20"},
{"22CH","22"},
{"24CH","24"},
{"26CH","26"},
{"28CH","28"},
{"30CH","30"},
{"32CH","32"}};

/*indexes*/
static char tick_idx,row_idx, pattern_idx;

/*offsets*/
static long tick_offset, buffer_offset;

/*lengths*/
static long tick_length;
static char row_length, pattern_length;

static struct ProtrackerChannel channels[32]:

char* ProtrackerGetNote(unsigned short period) {
	char* note;
	int i;
	i=0;
	if(period==0) {
		note = "---";
	} else {
		while(i<60 && ProtrackerPeriods[i] != period) {
			i++;}
		if(i>=60) {
			note = "???";
		} else {
			note = ProtrackerNotes[i];
		}
	}
	return note;
}

char ProtrackerGetPatternCount(ProtrackerModule *mod) {
	unsigned char i;
	unsigned char max;
	max =0;
	for(i=0;i<mod->songlength;i++) {
		if(mod->sequence[i] >= max) {
			max = mod->sequence[i];
		}
	}
	return max + 1;
}

char ProtrackerGetChannelCount(ProtrackerModule *mod) {
	unsigned char nbchan;
	int i;
	nbchan =0;
	for(i=0;i<21;i++) {
		if(strcmp(mod->type,ProtrackerTypes[i][0])) {
			nbchan = (char)atoi(ProtrackerTypes[i][1]);
			break;
		}
	}
	return nbchan;
}

long ProtrackerGetSamplesCount(ProtrackerModule *mod) {
	return 96000;
}

void ProtrackerGetAudio(short *output, long count) {
	long offset, remain;
	offset = 0;
	while (count>0) {
		remain = tick_length - tick_offset;
		if(remain >count) remain = count;
		
		tick_offset += remain;
		
		offset += remain;
		count -= remain;
	}
}

void ProtrackerInit(long sampling_frequency) {
	tick_idx = row_idx =  pattern_idx = 0;
	tick_offset = buffer_offset = 0;
	tick_length = sampling_frequency / 125;
	row_length = 6;
	pattern_length = 64;
}

