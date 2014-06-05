#include "protracker.h"

int ProtrackerGetPatternCount(ProtrackerModule *mod) {
	int i;
	char max;
	max =0;
	for(i=0;i<mod->songlength;i++) {
		if(mod->sequence[i] >= max) {
			max = mod->sequence[i];
		}
	}
	return max;
}

char* ProtrackerGetNote(short period) {
	float ratio = 1.0594f;
	float temp;
	char* note;
	temp = (float)period/428;
	if((int)temp==1) {
		note = "octave 2";
	} else if((int)temp == 0) {
		note = "octave 1";
	} else if((int)temp == 2) {
		note = "octave 3";
	} else {
		note = "";
	}
	return note;
}