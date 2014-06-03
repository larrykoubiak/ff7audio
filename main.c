#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "protracker.h"
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
	ProtrackerModule *mod;
	/*open file*/
	file = fopen(filename, "rb");
	if (file == NULL) {
		fprintf(stderr, "Unable to open file.\n");
		exit(EXIT_FAILURE);
	}
	/*parse file*/
	mod = malloc(sizeof(ProtrackerModule));
	mod->samples = malloc(sizeof(ProtrackerSample)*31);
	fread(mod,sizeof(ProtrackerModule),1,file);
	print_module(mod);
	/*clean up*/
	free(mod);
	error = fclose(file);
	if(error!=0) {
		fprintf(stderr, "Unable to close file.\n");
		exit(EXIT_FAILURE);
	}
}

void print_module(ProtrackerModule *mod) {
	fprintf(stdout, "Song name: %s\n", mod->songname);
	fprintf(stdout, "Sample name: %s\n", mod->samples[0]->name);
	fprintf(stdout, "Sample length: %s\n", mod->samples[0]->length);
}