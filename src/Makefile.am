AM_CFLAGS = $(warn_flags)
bin_PROGRAMS = noiser denoiser general_filter
lib_LTLIBRARIES = libpbm.la libpbmcore.la libpbmcodec.la libpbmfilter.la

noiser_SOURCES = noiser_main.c
noiser_LDADD = libpbm.la

denoiser_SOURCES = denoiser_main.c
denoiser_LDADD = libpbm.la

general_filter_SOURCES = general_main.c
general_filter_LDADD = libpbm.la

libpbm_la_SOURCES = pbmutil.c
libpbm_la_LIBADD = libpbmfilter.la libpbmcodec.la libpbmcore.la

libpbmfilter_la_SOURCES = filter/filter_common.c \
						  filter/filter_icm.c \
						  filter/filter_icm2.c \
						  filter/filter_noise.c \
						  filter/filter_diff.c \
						  filter/filters.c
libpbmfilter_la_LIBADD = libpbmcore.la $(mathlib_LIBS)
libpbmcodec_la_SOURCES = codec/codecs.c \
						 codec/codec_pbm.c \
						 codec/codec_count.c
if USE_LIBPNG
libpbmcodec_la_SOURCES+= codec/codec_png.c
endif
if USE_LIBSIXEL
libpbmcodec_la_SOURCES+= codec/codec_sixel.c
endif
if USE_GTK
libpbmcodec_la_SOURCES+= codec/codec_gtk.c
endif
if USE_OPENCV
libpbmcodec_la_SOURCES+= codec/codec_cv.c
endif
libpbmcodec_la_CFLAGS = $(AM_CFLAGS) $(libpng_CFLAGS) $(libsixel_CFLAGS) $(gtk_CFLAGS) $(opencv_CFLAGS)
libpbmcodec_la_LIBADD = libpbmcore.la $(libpng_LIBS) $(libsixel_LIBS) $(gtk_LIBS) $(opencv_LIBS)
libpbmcore_la_SOURCES = pbm.c \
						logger.c
