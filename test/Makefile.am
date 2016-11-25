AM_CFLAGS = $(warn_flags) -isystem $(top_srcdir)/externals/cutest -I$(top_srcdir)/src '-Dtestdatadir="$(srcdir)/data"'
if DEBUG
AM_TESTS_ENVIRONMENT = \
	LSAN_OPTIONS=suppressions=$(top_srcdir)/misc/lsan_ignore.txt; \
	export LSAN_OPTIONS;
else
AM_TESTS_ENVIRONMENT =
endif
LDADD = ../src/libpbm.la ../externals/libcutest.a

TESTS = pbm_test
check_PROGRAMS = pbm_test

pbm_test_SOURCES = pbm_test.c
