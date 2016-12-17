AM_CFLAGS = $(warn_flags) -isystem $(top_srcdir)/externals/cutest -I$(top_srcdir)/src '-Dtestdatadir="$(srcdir)/data"'
if DEBUG
AM_TESTS_ENVIRONMENT = \
	LSAN_OPTIONS=suppressions=$(top_srcdir)/misc/lsan_ignore.txt; \
	export LSAN_OPTIONS
else
AM_TESTS_ENVIRONMENT =
endif
AM_TESTS_ENVIRONMENT+= \
	EXEEXT=$(EXEEXT); \
	srcdir=$(srcdir); \
	export EXEEXT srcdir;

TESTS = pbmcore_test \
		pbmcodec_test \
		pbmfilter_test
if !cross_compiling
TESTS +=codec/codec_count_test.sh \
		general_filter_test.sh
endif
check_PROGRAMS = pbmcore_test pbmcodec_test pbmfilter_test
LDADD = ../externals/libcutest.a

pbmcore_test_SOURCES = test_main.c \
					   test_pbmcore.c \
					   pbm_test.c
pbmcore_test_LDADD = ../src/libpbmcore.la $(LDADD)
pbmcodec_test_SOURCES = test_main.c \
						codec/test_pbmcodec.c \
						codec/codec_pbm_test.c \
						codec/codec_png_test.c
pbmcodec_test_LDADD = ../src/libpbmcodec.la $(LDADD)
pbmfilter_test_SOURCES = test_main.c \
						 filter/test_pbmfilter.c \
						 filter/filter_noise_test.c \
						 filter/filter_diff_test.c \
						 filter/filter_icm_test.c \
						 filter/filter_icm2_test.c
pbmfilter_test_LDADD = ../src/libpbmfilter.la $(LDADD)
