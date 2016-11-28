#include "config.inc.h"

#include "test_pbmcodec.h"
#include <stdio.h>
#include "codec/codecs.h"
#include "logger.h"

#ifndef testdatadir
#define testdatadir "data"
#endif

#ifdef USE_LIBPNG
static void check_png_load(CuTest* tc, pbm_error_t err, const char* file) {
	LOG(info, "checking %s...", file);
	FILE* fp = fopen(file, "r");
	CuAssertPtrNotNull(tc, fp);

	pbm_info info;
	pbm_init(&info);
	pbm_error_t result = pbmcodec_png_read(&info, fp);
	CuAssertIntEquals(tc, err, result);
	if (result == PBM_SUCCESS) {
		CuAssertPtrNotNull(tc, info.data);
		CuAssertIntEquals(tc, 2, info.width);
		CuAssertIntEquals(tc, 2, info.height);
		CuAssertIntEquals(tc, 1, info.data[0][0]);
		CuAssertIntEquals(tc, 1, info.data[0][1]);
		CuAssertIntEquals(tc, 1, info.data[1][0]);
		CuAssertIntEquals(tc, 1, info.data[1][1]);
	}
	pbm_free(&info);
}

static void test_pbmcodec_png_read(CuTest* tc) {
#define CHECK_PNG_LOAD(expected, file) check_png_load(tc, expected, testdatadir "/" file)
	pbm_info info;
	pbm_init(&info);
	CHECK_PNG_LOAD(PBMCODEC_INVALID_SIGNATURE, "png/01_empty.png");
	CHECK_PNG_LOAD(PBMCODEC_INVALID_SIGNATURE, "png/01_invalid_sig1.png");
	CHECK_PNG_LOAD(PBMCODEC_INVALID_SIGNATURE, "png/01_invalid_sig2.png");
	CHECK_PNG_LOAD(PBM_SYSTEM_ERROR, "png/01_broken.png");
	CHECK_PNG_LOAD(PBM_SUCCESS, "png/02_gray.png");
	CHECK_PNG_LOAD(PBM_SUCCESS, "png/02_gray_alpha.png");
	CHECK_PNG_LOAD(PBM_SUCCESS, "png/02_indexed.png");
	CHECK_PNG_LOAD(PBM_SUCCESS, "png/02_rgb.png");
	CHECK_PNG_LOAD(PBM_SUCCESS, "png/02_rgb_alpha.png");
}

static void test_pbmcodec_png_write(CuTest* tc) {
	pbm_info info;
	pbm_init(&info);
	pbm_resize(&info, 3, 3);
	info.data[0][0] = 0;
	info.data[0][1] = 1;
	info.data[0][2] = 1;
	info.data[1][0] = 1;
	info.data[1][1] = 0;
	info.data[1][2] = 0;
	info.data[2][0] = 1;
	info.data[2][1] = 1;
	info.data[2][2] = 0;
#ifdef _WIN32
	FILE* fp = fopen("pbm_test.write.tmp", "w+");
#else
	FILE* fp = tmpfile();
#endif
	CuAssertPtrNotNull(tc, fp);
	CuAssertIntEquals(tc, PBM_SUCCESS, pbmcodec_png_write(&info, fp));
	fseek(fp, 0, SEEK_SET);
	pbmcodec_png_read(&info, fp);
	CuAssertIntEquals(tc, 3, info.width);
	CuAssertIntEquals(tc, 3, info.height);
	CuAssertIntEquals(tc, 0, info.data[0][0]);
	CuAssertIntEquals(tc, 1, info.data[0][1]);
	CuAssertIntEquals(tc, 1, info.data[0][2]);
	CuAssertIntEquals(tc, 1, info.data[1][0]);
	CuAssertIntEquals(tc, 0, info.data[1][1]);
	CuAssertIntEquals(tc, 0, info.data[1][2]);
	CuAssertIntEquals(tc, 1, info.data[2][0]);
	CuAssertIntEquals(tc, 1, info.data[2][1]);
	CuAssertIntEquals(tc, 0, info.data[2][2]);
	pbm_free(&info);
}

#endif

CuSuite* get_pngcodec_test_suites() {
	CuSuite* suite = CuSuiteNew();
#ifdef USE_LIBPNG
	SUITE_ADD_TEST(suite, test_pbmcodec_png_read);
	SUITE_ADD_TEST(suite, test_pbmcodec_png_write);
#endif
	return suite;
}
