#include "config.inc.h"

#include "test_pbmfilter.h"
#include <string.h>
#include "filter/filters.h"

static void test_pbmfilter_icm(CuTest* tc) {
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
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_icm(&a, &b, args));
	CuAssertIntEquals(tc, 0, b.data[30][30]);
	CuAssertIntEquals(tc, 0, b.data[30][60]);
	CuAssertIntEquals(tc, 0, b.data[60][30]);
	CuAssertIntEquals(tc, 0, b.data[60][60]);

	pbm_free(&a);
	pbm_free(&b);
}

CuSuite* get_icm_filter_test_suites() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_pbmfilter_icm);
	return suite;
}
