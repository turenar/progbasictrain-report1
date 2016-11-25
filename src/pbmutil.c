#include "config.inc.h"

#include "pbmutil.h"
#include <stdlib.h>
#include <string.h>
#include "logger.h"


pbm_error_t pbm_parse_filter_str(const char* str, filter_info* filter) {
	size_t arg_count = 0;
	size_t args_len = 0;
	for (const char* p = str; *p != '\0'; p++) {
		if (*p == ':') {
			arg_count++;
		}
		args_len++;
	}

	char* args_str = (char*) malloc(sizeof(char) * (args_len + 1));
	strcpy(args_str, str);

	char** filter_args = (char**) malloc(sizeof(char*) * arg_count);
	char** args_p = filter_args;
	*args_p++ = args_str;
	for (char* p = args_str; *p != '\0'; p++) {
		if (*p == ':') {
			*p++ = '\0';
			*args_p++ = p;
		}
	}
	*args_p = NULL;

	pbmfilter_fn filter_fn = pbmfilter_get_filter(filter_args[0]);
	if (!filter_fn) {
		LOG(error, "filter not found: '%s'", filter_args[0]);
		return PBMFILTER_NOT_FOUND;
	}

	filter->fn = filter_fn;
	filter->args = filter_args;
	return PBM_SUCCESS;
}

pbm_error_t pbm_free_filter_info(filter_info* filter) {
	free(filter->args[0]);
	free(filter->args);
	return PBM_SUCCESS;
}
