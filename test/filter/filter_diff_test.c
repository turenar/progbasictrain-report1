#include "config.inc.h"

#include "test_pbmfilter.h"
#include "filter/filters.h"

static void test_pbmfilter_diff(CuTest* tc) {
	pbm_info a;
	pbm_info b;
	pbm_info tmp1;
	pbm_info tmp2;
	pbm_init(&a);
	pbm_init(&b);
	pbm_init(&tmp1);
	pbm_init(&tmp2);
	pbm_resize(&a, 2, 2);
	a.data[0][0] = 1;
	a.data[0][1] = 1;
	a.data[1][0] = 1;
	a.data[1][1] = 1;
	pbm_resize(&b, 2, 2);
	b.data[0][0] = 1;
	b.data[0][1] = 0;
	b.data[1][0] = 0;
	b.data[1][1] = 1;

	char* args[] = {NULL};

	// diff between a and a
	pbm_copy(&a, &tmp1);
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_diff1(&tmp1, &tmp2, args));
	CuAssertIntEquals(tc, 2, tmp2.width);
	CuAssertIntEquals(tc, 2, tmp2.height);
	CuAssertIntEquals(tc, 1, tmp2.data[0][0]);
	CuAssertIntEquals(tc, 1, tmp2.data[0][1]);
	CuAssertIntEquals(tc, 1, tmp2.data[1][0]);
	CuAssertIntEquals(tc, 1, tmp2.data[1][1]);

	CuAssertIntEquals(tc, PBMFILTER_INVALID_ARG, pbmfilter_diff1(&tmp2, &tmp1, args));

	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_diff2(&tmp2, &tmp1, args));
	CuAssertIntEquals(tc, 2, tmp1.width);
	CuAssertIntEquals(tc, 2, tmp1.height);
	CuAssertIntEquals(tc, 0, tmp1.data[0][0]);
	CuAssertIntEquals(tc, 0, tmp1.data[0][1]);
	CuAssertIntEquals(tc, 0, tmp1.data[1][0]);
	CuAssertIntEquals(tc, 0, tmp1.data[1][1]);

	// diff between a and b
	CuAssertIntEquals(tc, PBMFILTER_INVALID_ARG, pbmfilter_diff2(&a, &tmp1, args));
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_diff1(&a, &tmp1, args));
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_diff2(&b, &tmp2, args));
	CuAssertIntEquals(tc, 2, tmp2.width);
	CuAssertIntEquals(tc, 2, tmp2.height);
	CuAssertIntEquals(tc, 0, tmp2.data[0][0]);
	CuAssertIntEquals(tc, 1, tmp2.data[0][1]);
	CuAssertIntEquals(tc, 1, tmp2.data[1][0]);
	CuAssertIntEquals(tc, 0, tmp2.data[1][1]);

	pbm_free(&a);
	pbm_free(&b);
	pbm_free(&tmp1);
	pbm_free(&tmp2);
}

static void test_pbmfilter_diff_size_mismatch(CuTest* tc) {
	pbm_info a;
	pbm_info b;
	pbm_info tmp1;
	pbm_info tmp2;
	pbm_init(&a);
	pbm_init(&b);
	pbm_init(&tmp1);
	pbm_init(&tmp2);
	pbm_resize(&a, 2, 2);
	a.data[0][0] = 1;
	a.data[0][1] = 1;
	a.data[1][0] = 1;
	a.data[1][1] = 1;
	pbm_resize(&b, 3, 3);

	char* args[] = {NULL};

	// diff a == b
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_diff1(&a, &tmp1, args));
	CuAssertIntEquals(tc, PBMFILTER_INVALID_ARG, pbmfilter_diff2(&b, &tmp2, args));
	pbm_free(&a);
	pbm_free(&b);
	pbm_free(&tmp1);
	pbm_free(&tmp2);
}

CuSuite* get_diff_filter_test_suites() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_pbmfilter_diff);
	SUITE_ADD_TEST(suite, test_pbmfilter_diff_size_mismatch);
	return suite;
}
