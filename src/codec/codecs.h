#pragma once

#include <stdio.h>
#include "../pbm.h"

typedef pbm_error_t (* pbmcodec_reader_fn)(pbm_info*, FILE*);
typedef pbm_error_t (* pbmcodec_writer_fn)(const pbm_info*, FILE*);

pbmcodec_reader_fn pbmcodec_get_reader(const char* name);
pbmcodec_writer_fn pbmcodec_get_writer(const char* name);
void pbmcodec_show_help(FILE*);

pbm_error_t pbmcodec_pbm_read(pbm_info*, FILE*);
pbm_error_t pbmcodec_pbm_write(const pbm_info*, FILE*);

pbm_error_t pbmcodec_count(const pbm_info*, FILE*);

#ifdef USE_LIBPNG
pbm_error_t pbmcodec_png_read(pbm_info*, FILE*);
pbm_error_t pbmcodec_png_write(const pbm_info*, FILE*);
#endif

#ifdef USE_LIBSIXEL
//pbm_error_t pbmcodec_sixel_read(pbm_info*, FILE*);
pbm_error_t pbmcodec_sixel_write(const pbm_info*, FILE*);
#endif

#ifdef USE_GTK
pbm_error_t pbmcodec_gtk_write(const pbm_info*, FILE*);
#endif

#ifdef USE_OPENCV
pbm_error_t pbmcodec_cv_write(const pbm_info*, FILE*);
#endif
