#pragma once

#include "filter/filters.h"

typedef struct {
	pbmfilter_fn fn;
	char** args;
} filter_info;

pbm_error_t pbm_parse_filter_str(char*, filter_info*);
