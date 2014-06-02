#include <stdio.h>
#include <stdlib.h>

#include "main.h"

int main(int argc, char **argv) {
	char *filename;
	if(argc == 2) {
		filename = argv[1];
	} else if(argc ==3 && strcmp(argv[1],"-reverb")) == 0 {
		filename = argv[2];
	} else {
		fprintf(stderr,"Usage: %s [-reverb] filename.\n",argv[0]);	
		return EXIT_FAILURE;	
	}
	fprintf(stdout,"%s is now open!",filename);	
	return EXIT_SUCCESS;
}
