#include "config.inc.h"

#include "test_pbmfilter.h"
#include <string.h>
#include "filter/filters.h"

static void test_pbmfilter_icm2(CuTest* tc) {
	pbm_info a;
	pbm_info b;
	pbm_init(&a);
	pbm_init(&b);
	pbm_resize(&a, 100, 100);

	uint8_t** row_p = a.data;
	for (int y = 0; y < a.height; ++y) {
		memset(*row_p++, 0, sizeof(uint8_t) * (size_t) a.width);
	}

	a.data[30][30] = 1;
	a.data[30][60] = 1;
	a.data[60][30] = 1;
	a.data[60][60] = 1;

	char* args[] = {NULL};
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_icm2(&a, &b, args));
	CuAssertIntEquals(tc, 0, b.data[30][30]);
	CuAssertIntEquals(tc, 0, b.data[30][60]);
	CuAssertIntEquals(tc, 0, b.data[60][30]);
	CuAssertIntEquals(tc, 0, b.data[60][60]);

	pbm_free(&a);
	pbm_free(&b);
}

static void test_pbmfilter_icm2_equals_icm_without_args(CuTest* tc) {
	pbm_info orig;
	pbm_info in;
	pbm_info res1;
	pbm_info res2;
	pbm_init(&orig);
	pbm_init(&in);
	pbm_init(&res1);
	pbm_init(&res2);
	pbm_resize(&orig, 100, 100);

	uint8_t** row_p = orig.data;
	for (int y = 0; y < orig.height; ++y) {
		memset(*row_p++, 0, sizeof(uint8_t) * (size_t) orig.width);
	}


	char* args[] = {NULL};
	char buf[256];
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_noise(&orig, &in, args));

	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_icm(&in, &res1, args));
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_icm2(&in, &res2, args));
	uint8_t** row1_p = res1.data;
	uint8_t** row2_p = res2.data;
	for (int y = 0; y < orig.height; ++y) {
		uint8_t* col1_p = *row1_p++;
		uint8_t* col2_p = *row2_p++;
		for (int x = 0; x < orig.width; ++x) {
			sprintf(buf, "%d,%d", x, y);
			CuAssertIntEquals_Msg(tc, buf, *col1_p++, *col2_p++);
		}
	}

	pbm_free(&orig);
	pbm_free(&in);
	pbm_free(&res1);
	pbm_free(&res2);
}

CuSuite* get_icm2_filter_test_suites() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_pbmfilter_icm2);
	SUITE_ADD_TEST(suite, test_pbmfilter_icm2_equals_icm_without_args);
	return suite;
}
