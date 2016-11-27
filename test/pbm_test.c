#include "config.inc.h"

#include "test_pbmcore.h"
#include "codec/codecs.h"

#ifndef testdatadir
#define testdatadir "data"
#endif

static void test_pbm_init(CuTest* tc) {
	pbm_info info;
	pbm_init(&info);
	CuAssertPtrEquals(tc, NULL, info.data);
}

static void test_pbm_resize(CuTest* tc) {
	pbm_info info;
	pbm_init(&info);
	pbm_resize(&info, 2, 2);
	CuAssertIntEquals(tc, 2, info.width);
	CuAssertIntEquals(tc, 2, info.height);
	CuAssertPtrNotNull(tc, info.data);
	// should free check... but, -fsanitizer=leak can do...?
	pbm_resize(&info, 1, 1);
	CuAssertIntEquals(tc, 1, info.width);
	CuAssertIntEquals(tc, 1, info.height);
	CuAssertPtrNotNull(tc, info.data);
	pbm_free(&info);
}

static void test_pbm_free(CuTest* tc) {
	pbm_info info;
	pbm_init(&info);
	pbm_free(&info);
	CuAssertPtrEquals(tc, NULL, info.data);

	pbm_resize(&info, 2, 2);
	pbm_free(&info);
	CuAssertPtrEquals(tc, NULL, info.data);
}

static void test_pbm_copy(CuTest* tc) {
	pbm_info a;
	pbm_info b;
	pbm_init(&a);
	pbm_init(&b);
	pbm_resize(&a, 2, 2);
	a.data[0][0] = 1;
	a.data[0][1] = 0;
	a.data[1][0] = 0;
	a.data[1][1] = 1;

	pbm_copy(&a, &b);
	CuAssertIntEquals(tc, 2, b.width);
	CuAssertIntEquals(tc, 2, b.height);
	CuAssertIntEquals(tc, 1, b.data[0][0]);
	CuAssertIntEquals(tc, 0, b.data[0][1]);
	CuAssertIntEquals(tc, 0, b.data[1][0]);
	CuAssertIntEquals(tc, 1, b.data[1][1]);

	pbm_free(&a);
	pbm_free(&b);
}

CuSuite* get_pbm_test_suites() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_pbm_init);
	SUITE_ADD_TEST(suite, test_pbm_copy);
	SUITE_ADD_TEST(suite, test_pbm_free);
	SUITE_ADD_TEST(suite, test_pbm_resize);
	return suite;
}
