#pragma once

#include <stdio.h>
#include "pbm.h"

pbm_error_t codec_pbm_read(pbm_info*, FILE*);
pbm_error_t codec_pbm_write(const pbm_info*, FILE*);

#ifdef USE_LIBPNG
pbm_error_t codec_png_read(pbm_info*, FILE*);
pbm_error_t codec_png_write(const pbm_info*, FILE*);
#endif
