#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
int main(int argc, char **argv) {
	char *filename;
	ProtrackerModule *mod;
	if(argc == 2) {
		filename = argv[1];
	} else if(argc ==3 && strcmp(argv[1],"-reverb") == 0) {
		reverb_len = REVERB_BUF_LEN;
		filename = argv[2];
	} else {
		fprintf(stderr,"Usage: %s [-reverb] filename.\n",argv[0]);	
		return EXIT_FAILURE;	
	}
	mod = malloc(sizeof(ProtrackerModule));
	load_module(filename, mod);
	print_module(mod);
	free_module(mod);
	return EXIT_SUCCESS;
}

void load_module(char* filename, ProtrackerModule* mod) {
	FILE *file;
	long error;
	int i,j,k;
	unsigned char tempnote[4];
	ProtrackerPattern *pattern;
	ProtrackerNote *note;
	/*open file*/
	file = fopen(filename, "rb");
	if (file == NULL) {
		fprintf(stderr, "Unable to open file.\n");
		exit(EXIT_FAILURE);
	}
	/*----------*/
	/*parse file*/
	/*----------*/	
	fread(mod->songname,20,1,file);
	/*parse samples data*/
	for(i=0;i<31;i++) {
		mod->samples[i] = malloc(sizeof(ProtrackerSample));
		fread(mod->samples[i],sizeof(ProtrackerSample),1,file);
		mod->samples[i]->length = swap16(mod->samples[i]->length) * 2;
		mod->samples[i]->repeat = swap16(mod->samples[i]->repeat) * 2;
		mod->samples[i]->replen = swap16(mod->samples[i]->replen) * 2;
	}
	/* read mod info */
	fread(&(mod->songlength),1,1,file);
	fread(&(mod->reset),1,1,file);
	fread(mod->sequence,1,128,file);
	memset(mod->type,0,5);
	fread(mod->type,1,4,file);
	/* count channels */
	mod->nbChannels = ProtrackerGetChannelCount(mod);
	/* read pattern data*/
	mod->nbPatterns = ProtrackerGetPatternCount(mod);
	mod->patterns = malloc(sizeof(ProtrackerPattern)*mod->nbPatterns);
	pattern = mod->patterns;
	for(i=0; i<mod->nbPatterns;i++) {
		pattern = mod->patterns+i;
		pattern->notes = malloc(sizeof(ProtrackerNote)*64*mod->nbChannels);
		for(j=0; j<64;j++) {
			for(k=0; k<mod->nbChannels;k++) {
				fread(tempnote,sizeof(char),4,file);
				note = pattern->notes+((j*mod->nbChannels)+k);
				note->sample_idx = (tempnote[0] & 0xF0) + (tempnote[2] >> 4);
				note->period = tempnote[1] | ((tempnote[0] & 0X0F) <<8);
				note->effect = tempnote[3] | ((tempnote[2] & 0x0F) <<8);
			}
		}
	}
	error = fclose(file);
	if(error!=0) {
		fprintf(stderr, "Unable to close file.\n");
		exit(EXIT_FAILURE);
	}
}

void free_module(ProtrackerModule *mod) {
	int i;
	ProtrackerPattern *pattern;
	for(i=0;i<mod->nbPatterns;i++) {
		pattern = mod->patterns+i;
		free(pattern->notes);
	}
	free(mod->patterns);
	for(i=0;i<31;i++) {
		free(mod->samples[i]);
	}
	free(mod);
}

void print_module(ProtrackerModule *mod) {
	int i,j,k;
	ProtrackerPattern *pattern;
	ProtrackerNote *note;
	fprintf(stdout, "Song name   	: %s\n", mod->songname);
	fprintf(stdout, "Song length 	: %4i\n", mod->songlength);
	fprintf(stdout, "Song reset  	: %4i\n", mod->reset);
	fprintf(stdout, "Song type   	: %4s\n", mod->type);
	fprintf(stdout, "Song pat/chan	: %2i/%2i\n", mod->nbPatterns,mod->nbChannels);
	fprintf(stdout, ".%20s.%6s.%3s.%3s.%5s.%5s.\n","--------------------","------","---","---","-----","-----");
	fprintf(stdout, "|%20s|%6s|%3s|%3s|%5s|%5s|\n","Sample name","LEN", "FT", "VOL", "LOOP", "REPL");
	fprintf(stdout, ".%20s.%6s.%3s.%3s.%5s.%5s.\n","--------------------","------","---","---","-----","-----");
	for (i=0;i<31;i++) {
		ProtrackerSample *smp;
		smp = mod->samples[i];	
		fprintf(stdout, "|%20s|%6i|%3i|%3i|%5i|%5i|\n", smp->name, smp->length, smp->finetune, smp->volume, smp->repeat, smp->replen);
	}
	fprintf(stdout, ".%20s.%6s.%3s.%3s.%5s.%5s.\n","--------------------","------","---","---","-----","-----");
	fprintf(stdout, "Sequence : ");
	for(i=0;i<mod->songlength;i++) {
		fprintf(stdout, "%02i ",mod->sequence[i]);
	}
	fprintf(stdout, "\n");
	for (i=0;i<mod->nbPatterns;i++) {
		pattern = mod->patterns+i;
		fprintf(stdout,".-------------------------------------------------------------.\n");
		for(j=0;j<64;j++) {
			fprintf(stdout,"| %02i | %02X | ",i,j);
			for(k=0;k<mod->nbChannels;k++) {
				note = pattern->notes+((j*mod->nbChannels)+k);
				fprintf(stdout, "%3s %02X %03X | ", ProtrackerGetNote(note->period), note->sample_idx, note->effect);
			}
			fprintf(stdout,"\n");
		}
	}
	fprintf(stdout,".-------------------------------------------------------------.\n");
}