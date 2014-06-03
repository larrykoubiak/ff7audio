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
	int i;
	ProtrackerModule *mod;
	/*open file*/
	file = fopen(filename, "rb");
	if (file == NULL) {
		fprintf(stderr, "Unable to open file.\n");
		exit(EXIT_FAILURE);
	}
	/*parse file*/
	mod = malloc(sizeof(ProtrackerModule));
	fread(mod,sizeof(ProtrackerModule),1,file);
	/*parse samples*/
	mod->samples = malloc(sizeof(ProtrackerSample)*31);
	fseek(file,20,SEEK_SET);
	fread(mod->samples,sizeof(ProtrackerSample),31,file);
	for(i=0;i<31;i++) {
		mod->samples[i].length = swap16(mod->samples[i].length) * 2;
		mod->samples[i].repeat = swap16(mod->samples[i].repeat) * 2;
		mod->samples[i].replen = swap16(mod->samples[i].replen) * 2;
	}
	print_module(mod);
	/*clean up*/
	free(mod->samples);
	free(mod);
	error = fclose(file);
	if(error!=0) {
		fprintf(stderr, "Unable to close file.\n");
		exit(EXIT_FAILURE);
	}
}

void print_module(ProtrackerModule *mod) {
	int i;
	fprintf(stdout, "\nSong name: %s\n\n", mod->songname);
	fprintf(stdout, "%20s %6s %3s %3s %5s %5s\n","Sample name","LEN", "FT", "VOL", "LOOP", "REPL");
	fprintf(stdout, "%20s %6s %3s %3s %5s %5s\n","--------------------","------","---","---","---","-----","-----");
	for (i=0;i<31;i++) {
		ProtrackerSample smp;
		smp = mod->samples[i];	
		fprintf(stdout, "%20s %6i %3i %3i %5i %5i\n", smp.name, smp.length, smp.finetune, smp.volume, smp.repeat, smp.replen);
	}
}