#pragma once

#include <stdio.h>
#include "pbm.h"

typedef pbm_error_t (* pbmcodec_reader_fn)(pbm_info*, FILE*);
typedef pbm_error_t (* pbmcodec_writer_fn)(const pbm_info*, FILE*);

pbmcodec_reader_fn pbmcodec_get_reader(const char* name);
pbmcodec_writer_fn pbmcodec_get_writer(const char* name);

pbm_error_t pbmcodec_pbm_read(pbm_info*, FILE*);
pbm_error_t pbmcodec_pbm_write(const pbm_info*, FILE*);

#ifdef USE_LIBPNG
pbm_error_t pbmcodec_png_read(pbm_info*, FILE*);
pbm_error_t pbmcodec_png_write(const pbm_info*, FILE*);
#endif
