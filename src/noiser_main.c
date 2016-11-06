#include "config.inc.h"
#include <stdio.h>
#include "pbm.h"


int main(int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "usage: %s <input> <output>\n", argv[0]);
		return 1;
	}

	pbm_info input;
	FILE* rfp = fopen(argv[1], "r");
	FILE* wfp = fopen(argv[2], "w");
	if (pbm_load(&input, rfp)) {
		fprintf(stderr, "error\n");
		return 1;
	}
	pbm_write(&input, wfp);
	return 0;
}
