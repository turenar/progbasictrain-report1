#pragma once

#include "pbm.h"

int pbmfilter_noise(const pbm_info* in, pbm_info* out, const char** args);
int pbmfilter_icm(const pbm_info* in, pbm_info* out, const char** args);