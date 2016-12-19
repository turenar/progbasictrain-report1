#include "config.inc.h"

#include <stdio.h>
#include "codec/codecs.h"
#include "filter/filters.h"

int main(int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "usage: %s <input.pbm> <output.pbm>\n", argv[0]);
		return 1;
	}

	pbm_info input;
	pbm_init(&input);
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
	pbm_init(&output);
	// argc >= 3なのでargv[>=3]のどれかにはNULLが入っているらしい
	// icmは引数は使わないけど
	if (!pbmfilter_icm(&input, &output, argv + 3)) {
		if (!pbmcodec_pbm_write(&output, wfp)) {
			return 0;
		}
	}
	fprintf(stderr, "error\n");
	return 1;
}
