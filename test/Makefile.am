AM_CFLAGS = $(warn_flags) -isystem $(top_srcdir)/externals/cutest -I$(top_srcdir)/src '-Dtestdatadir="$(srcdir)/data"'
LDADD = ../src/libpbm.a ../externals/libcutest.a

TESTS = pbm_test
check_PROGRAMS = pbm_test

pbm_test_SOURCES = pbm_test.c
