AM_CFLAGS = $(warn_flags) $(libpng_CFLAGS) $(libsixel_CFLAGS)
bin_PROGRAMS = noiser denoiser
noinst_LIBRARIES = libpbm.a

noiser_SOURCES = noiser_main.c
noiser_LDADD = libpbm.a $(libpng_LIBS) $(libsixel_LIBS)

denoiser_SOURCES = denoiser_main.c
denoiser_LDADD = libpbm.a $(libpng_LIBS) $(libsixel_LIBS)

pbm_filters = filter/filter_common.c \
			  filter/filter_icm.c \
			  filter/filter_noise.c
pbm_codecs = codec/codecs.c \
			 codec/codec_pbm.c \
			 codec/codec_png.c \
			 codec/codec_sixel.c
pbm_cores = pbm.c logger.c
libpbm_a_SOURCES = $(pbm_cores) $(pbm_filters) $(pbm_codecs)
