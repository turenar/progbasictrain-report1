#include "config.inc.h"

#include "pbmutil.h"
#include <stdlib.h>
#include "logger.h"


pbm_error_t pbm_parse_filter_str(char* str, filter_info* filter) {
	size_t arg_count = 0;
	for (char* p = str; *p != '\0'; p++) {
		if (*p == ':') {
			arg_count++;
		}
	}
	char** filter_args = (char**) malloc(sizeof(char*) * arg_count);
	char** args_p = filter_args;
	*args_p++ = str;
	for (char* p = str; *p != '\0'; p++) {
		if (*p == ':') {
			*p++ = '\0';
			*args_p++ = p;
		}
	}

	pbmfilter_fn filter_fn = pbmfilter_get_filter(filter_args[0]);
	if (!filter_fn) {
		LOG(error, "filter not found: '%s'", filter_args[0]);
		return PBMFILTER_NOT_FOUND;
	}

	filter->fn = filter_fn;
	filter->args = filter_args;
	return PBM_SUCCESS;
}
