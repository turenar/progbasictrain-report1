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

