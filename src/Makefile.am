AM_CFLAGS = $(warn_flags) $(libpng_CFLAGS) $(libsixel_CFLAGS) $(gtk_CFLAGS)
bin_PROGRAMS = noiser denoiser general_filter
noinst_LIBRARIES = libpbm.a

noiser_SOURCES = noiser_main.c
noiser_LDADD = libpbm.a $(libpng_LIBS) $(libsixel_LIBS) $(gtk_LIBS)

denoiser_SOURCES = denoiser_main.c
denoiser_LDADD = libpbm.a $(libpng_LIBS) $(libsixel_LIBS) $(gtk_LIBS)

general_filter_SOURCES = general_main.c
general_filter_LDADD = libpbm.a $(libpng_LIBS) $(libsixel_LIBS) $(gtk_LIBS)

pbm_filters = filter/filter_common.c \
			  filter/filter_icm.c \
			  filter/filter_icm2.c \
			  filter/filter_noise.c \
			  filter/filter_diff.c \
			  filter/filters.c
pbm_codecs = codec/codecs.c \
			 codec/codec_pbm.c \
			 codec/codec_png.c \
			 codec/codec_sixel.c \
			 codec/codec_gtk.c
pbm_cores = pbm.c \
			pbmutil.c \
			logger.c
libpbm_a_SOURCES = $(pbm_cores) $(pbm_filters) $(pbm_codecs)
