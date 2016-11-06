#pragma once

#include "pbm.h"

typedef uint8_t(* pbmfilter_p2p_t)(uint8_t, void*);

void pbmfilter_apply(const pbm_info* in, pbm_info* out, pbmfilter_p2p_t filter, void*);
