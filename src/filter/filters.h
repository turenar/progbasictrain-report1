#pragma once

#include "../pbm.h"

typedef pbm_error_t (* pbmfilter_fn)(const pbm_info*, pbm_info*, const char**);

pbmfilter_fn pbmfilter_get_filter(const char* name);


pbm_error_t pbmfilter_noise(const pbm_info* in, pbm_info* out, const char** args);
pbm_error_t pbmfilter_icm(const pbm_info* in, pbm_info* out, const char** args);
