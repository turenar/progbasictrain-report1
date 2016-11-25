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
libpbmfilter_la_LIBADD = $(mathlib_LIBS)
libpbmcodec_la_SOURCES = codec/codecs.c \
						 codec/codec_pbm.c \
						 codec/codec_png.c \
						 codec/codec_sixel.c \
						 codec/codec_gtk.c \
						 codec/codec_count.c
libpbmcodec_la_CFLAGS = $(AM_CFLAGS) $(libpng_CFLAGS) $(libsixel_CFLAGS) $(gtk_CFLAGS)
libpbmcodec_la_LIBADD = $(libpng_LIBS) $(libsixel_LIBS) $(gtk_LIBS)
libpbmcore_la_SOURCES = pbm.c \
						logger.c
