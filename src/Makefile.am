AM_CFLAGS = $(warn_flags)
bin_PROGRAMS = noiser denoiser
noinst_LIBRARIES = libnoiser_core.a libnoiser_filter.a

noiser_SOURCES = noiser_main.c
noiser_LDADD = libnoiser_core.a libnoiser_filter.a

denoiser_SOURCES = denoiser_main.c
denoiser_LDADD = libnoiser_core.a libnoiser_filter.a

libnoiser_core_a_SOURCES = pbm.c

libnoiser_filter_a_SOURCES = filter/filter_noise.c filter/filter_common.c filter/filter_icm.c
