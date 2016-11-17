#include "config.inc.h"

#include "filters.h"
#include <string.h>

typedef struct {
	const char* name;
	pbmfilter_fn fn;
} pbmfilter_info;

static pbmfilter_info filters[] = {
		{"icm",   pbmfilter_icm},
		{"icm2",   pbmfilter_icm2},
		{"noise", pbmfilter_noise},
		{"diff1", pbmfilter_diff1},
		{"diff2", pbmfilter_diff2},
		{NULL, NULL}
};

pbmfilter_fn pbmfilter_get_filter(const char* name) {
	for (pbmfilter_info* p = filters; p->name != NULL; p++) {
		if (strcmp(p->name, name) == 0) {
			return p->fn;
		}
	}
	return NULL;
}

void pbmfilter_show_help(FILE* fp) {
	fprintf(fp, ""
			"  icm\n"
			"    denoise filter; iterated conditional modes\n"
			"  noise[:<prob>]\n"
			"    random whilte<->black flip in <prob> probability\n"
			"      args:     prob       flip probability [double]\n"
			"      example:  'noise' flips in 10%% chance\n"
			"                'noise:1.0' flips all pixels\n"
			"  diff1\n"
			"    save image data for diff2\n"
			"  diff2\n"
			"    output diff from _diff1_\n"
			"    you must call only once _diff2_ after once _diff1_\n");
}
