#include "config.inc.h"

#include "filters.h"
#include <string.h>

typedef struct {
	const char* name;
	pbmfilter_fn fn;
} pbmfilter_info;

static pbmfilter_info filters[] = {
		{"icm",   pbmfilter_icm},
		{"noise", pbmfilter_noise},
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
			"                'noise:1.0' flips all pixels\n");
}
