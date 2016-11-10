AM_CFLAGS = $(warn_flags)
bin_PROGRAMS = noiser denoiser
noinst_LIBRARIES = libpbm.a

noiser_SOURCES = noiser_main.c
noiser_LDADD = libpbm.a

denoiser_SOURCES = denoiser_main.c
denoiser_LDADD = libpbm.a

filters = filter/filter_common.c filter/filter_icm.c filter/filter_noise.c
codecs = codec/codec_pbm.c
libpbm_a_SOURCES = pbm.c $(filters) $(codecs)
