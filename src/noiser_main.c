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
	FILE* rfp = fopen(argv[1], "rb");
	if (!rfp) {
		perror(argv[1]);
		return 1;
	}
	FILE* wfp = fopen(argv[2], "wb");
	if (!wfp) {
		perror(argv[2]);
		return 1;
	}
	if (pbmcodec_pbm_read(&input, rfp)) {
		fprintf(stderr, "error\n");
		return 1;
	}

	pbm_info output;
	// argv[argc] == NULL
	pbmfilter_noise(&input, &output, (const char**) argv + 3);
	pbmcodec_pbm_write(&output, wfp);
	return 0;
}
