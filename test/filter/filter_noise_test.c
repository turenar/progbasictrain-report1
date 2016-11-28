#include "config.inc.h"

#include "test_pbmfilter.h"
#include <string.h>
#include "filter/filters.h"

static void test_pbmfilter_noise_default(CuTest* tc) {
	pbm_info in;
	pbm_info out;
	pbm_init(&in);
	pbm_init(&out);
	pbm_resize(&in, 100, 100);

	uint8_t** row_p = in.data;
	for (int y = 0; y < in.height; ++y) {
		memset(*row_p++, 0, sizeof(uint8_t) * (size_t) in.width);
	}

	char* args[] = {NULL};
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_noise(&in, &out, args));

	CuAssertIntEquals(tc, in.width, out.width);
	CuAssertIntEquals(tc, in.height, out.height);
	int c = 0;
	row_p = out.data;
	for (int y = 0; y < out.height; ++y) {
		uint8_t* col_p = *row_p++;
		for (int x = 0; x < out.width; ++x) {
			c += *col_p++;
		}
	}
	int size = out.width * out.height;
	CuAssert(tc, "", size * 0.05 < c && c < size * 0.15);
	pbm_free(&in);
	pbm_free(&out);
}

static void test_pbmfilter_noise_invert(CuTest* tc) {
	pbm_info in;
	pbm_info out;
	pbm_init(&in);
	pbm_init(&out);
	pbm_resize(&in, 100, 100);

	uint8_t** row_p = in.data;
	for (int y = 0; y < in.height; ++y) {
		memset(*row_p++, 0, sizeof(uint8_t) * (size_t) in.width);
	}

	char* args[] = {"1.0", NULL};
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_noise(&in, &out, args));

	CuAssertIntEquals(tc, in.width, out.width);
	CuAssertIntEquals(tc, in.height, out.height);
	int c = 0;
	row_p = out.data;
	for (int y = 0; y < out.height; ++y) {
		uint8_t* col_p = *row_p++;
		for (int x = 0; x < out.width; ++x) {
			c += *col_p++;
		}
	}
	int size = out.width * out.height;
	CuAssertIntEquals(tc, size, c);
	pbm_free(&in);
	pbm_free(&out);
}

static void test_pbmfilter_noise_asis(CuTest* tc) {
	pbm_info in;
	pbm_info out;
	pbm_init(&in);
	pbm_init(&out);
	pbm_resize(&in, 100, 100);

	uint8_t** row_p = in.data;
	for (int y = 0; y < in.height; ++y) {
		memset(*row_p++, 0, sizeof(uint8_t) * (size_t) in.width);
	}

	char* args[] = {"0", NULL};
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmfilter_noise(&in, &out, args));

	CuAssertIntEquals(tc, in.width, out.width);
	CuAssertIntEquals(tc, in.height, out.height);
	int c = 0;
	row_p = out.data;
	for (int y = 0; y < out.height; ++y) {
		uint8_t* col_p = *row_p++;
		for (int x = 0; x < out.width; ++x) {
			c += *col_p++;
		}
	}
	CuAssertIntEquals(tc, 0, c);
	pbm_free(&in);
	pbm_free(&out);
}

static void test_pbmfilter_noise_invalid_arg(CuTest* tc) {
	pbm_info in;
	pbm_info out;
	pbm_init(&in);
	pbm_init(&out);
	pbm_resize(&in, 100, 100);

	uint8_t** row_p = in.data;
	for (int y = 0; y < in.height; ++y) {
		memset(*row_p++, 0, sizeof(uint8_t) * (size_t) in.width);
	}

	char* args[] = {"invalid", NULL};
	CuAssertIntEquals(tc, PBMFILTER_INVALID_ARG, pbmfilter_noise(&in, &out, args));
	pbm_free(&in);
	pbm_free(&out);
}

CuSuite* get_noise_filter_test_suites() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_pbmfilter_noise_default);
	SUITE_ADD_TEST(suite, test_pbmfilter_noise_invert);
	SUITE_ADD_TEST(suite, test_pbmfilter_noise_asis);
	SUITE_ADD_TEST(suite, test_pbmfilter_noise_invalid_arg);
	return suite;
}
