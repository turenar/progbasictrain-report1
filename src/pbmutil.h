#pragma once

#include "filter/filters.h"

typedef struct {
	pbmfilter_fn fn;
	char** args;
} filter_info;

pbm_error_t pbm_parse_filter_str(const char*, filter_info*);
pbm_error_t pbm_free_filter_info(filter_info*);
