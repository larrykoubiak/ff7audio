#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
int main(int argc, char **argv) {
	char *filename;
	if(argc == 2) {
		filename = argv[1];
	} else if(argc ==3 && strcmp(argv[1],"-reverb") == 0) {
		reverb_len = REVERB_BUF_LEN;
		filename = argv[2];
	} else {
		fprintf(stderr,"Usage: %s [-reverb] filename.\n",argv[0]);	
		return EXIT_FAILURE;	
	}
	load_module(filename);
	return EXIT_SUCCESS;
}

void load_module(char* filename) {
	FILE *file;
	long error;
	int i,j,k;
	unsigned char tempnote[4];
	ProtrackerPattern *pattern;
	ProtrackerModule *mod;
	/*open file*/
	file = fopen(filename, "rb");
	if (file == NULL) {
		fprintf(stderr, "Unable to open file.\n");
		exit(EXIT_FAILURE);
	}
	/*----------*/
	/*parse file*/
	/*----------*/	
	mod = malloc(sizeof(ProtrackerModule));
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
	/* read pattern data*/
	mod->nbPatterns = ProtrackerGetPatternCount(mod);
	mod->patterns = malloc(sizeof(ProtrackerPattern)*mod->nbPatterns);
	pattern = mod->patterns;
	for(i=0; i<mod->nbPatterns;i++) {
		pattern = mod->patterns+i;
		for(j=0; j<64;j++) {
			for(k=0; k<4;k++) {
				fread(tempnote,sizeof(char),4,file);
				pattern->notes[k][j].sample_idx = (tempnote[0] & 0xF0) + (tempnote[2] >> 4);
				pattern->notes[k][j].period = tempnote[1] | ((tempnote[0] & 0X0F) <<8);
				pattern->notes[k][j].effect = tempnote[3] | ((tempnote[2] & 0x0F) <<8);
			}
		}
	}
	/*clean up*/
	free(mod->patterns);
	for(i=0;i<31;i++) {
		free(mod->samples[i]);
	}
	free(mod);
	error = fclose(file);
	if(error!=0) {
		fprintf(stderr, "Unable to close file.\n");
		exit(EXIT_FAILURE);
	}
}

void print_module(ProtrackerModule *mod) {
	int i,j,k;
	ProtrackerPattern *pattern;
	fprintf(stdout, "Song name   : %s\n", mod->songname);
	fprintf(stdout, "Song length : %4i\n", mod->songlength);
	fprintf(stdout, "Song reset  : %4i\n", mod->reset);
	fprintf(stdout, "Song type   : %s\n", mod->type);
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
		fprintf(stdout,".------------------------------------------------------------.\n");
		for(j=0;j<64;j++) {
			fprintf(stdout,"| %02i | %02X |",i,j);
			for(k=0;k<4;k++) {
				ProtrackerNote note;
				note = pattern->notes[k][j];
				fprintf(stdout, "%03s %02X %03X | ", ProtrackerGetNote(note.period), note.sample_idx, note.effect);
			}
			fprintf(stdout,"\n");
		}
	}
	fprintf(stdout,".------------------------------------------------------------.\n");
}