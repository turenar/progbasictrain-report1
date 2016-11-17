#pragma once

#include <stdio.h>
#include "../pbm.h"

typedef pbm_error_t (* pbmfilter_fn)(const pbm_info*, pbm_info*, char**);

pbmfilter_fn pbmfilter_get_filter(const char* name);
void pbmfilter_show_help(FILE*);

pbm_error_t pbmfilter_noise(const pbm_info* in, pbm_info* out, char** args);
pbm_error_t pbmfilter_icm(const pbm_info* in, pbm_info* out, char** args);
pbm_error_t pbmfilter_diff1(const pbm_info* in, pbm_info* out, char** args);
pbm_error_t pbmfilter_diff2(const pbm_info* in, pbm_info* out, char** args);
