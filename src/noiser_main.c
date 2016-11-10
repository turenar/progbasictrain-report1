#include "config.inc.h"

#include <stdio.h>
#include "codec/codecs.h"
#include "filter/filters.h"

int main(int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "usage: %s <input.pbm> <output.pbm> [<prob>]\n", argv[0]);
		return 1;
	}

	pbm_info input;
	FILE* rfp = fopen(argv[1], "r");
	if (!rfp) {
		perror(argv[1]);
		return 1;
	}
	FILE* wfp = fopen(argv[2], "w");
	if (!wfp) {
		perror(argv[2]);
		return 1;
	}
	if (codec_pbm_read(&input, rfp)) {
		fprintf(stderr, "error\n");
		return 1;
	}

	pbm_info output;
	pbmfilter_noise(&input, &output, (const char**) argv + 3);
	codec_pbm_write(&output, wfp);
	return 0;
}
