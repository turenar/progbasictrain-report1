#include "config.inc.h"

#include "filter_common.h"
#include <stdlib.h>
#include "logger.h"
#include "filters.h"

static uint8_t _pbmfilter_noise_point(uint8_t, void*);

pbm_error_t pbmfilter_noise(const pbm_info* in, pbm_info* out, char** args) {
	pbm_error_t res = pbm_resize(out, in->width, in->height);
	if (res) {
		return res;
	}

	double prob = 0.1;
	char* arg = args[0];
	if (arg != NULL) {
		char* endptr;
		if (arg[0] == '\0') {
			LOG(error, "invalid argument[0]: empty");
			return PBMFILTER_INVALID_ARG;
		}
		prob = strtod(arg, &endptr);
		if (*endptr != '\0') {
			LOG(error, "invalid argument[0]: not double (%s)", arg);
			return PBMFILTER_INVALID_ARG;
		} else if (prob < 0. || 1. < prob) {
			LOG(error, "pbmfilter_noise: invalid argument[0]: not probability (%s)", arg);
			return PBMFILTER_INVALID_ARG;
		}
	}
	pbmfilter_apply(in, out, _pbmfilter_noise_point, (void*) &prob);
	return PBM_SUCCESS;
}

static uint8_t _pbmfilter_noise_point(uint8_t in, void* data) {
	double prob = *((double*) data);
	if (rand() < RAND_MAX * prob) {
		return (uint8_t) !in;
	} else {
		return in;
	}
}
