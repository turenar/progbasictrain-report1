#include "config.inc.h"

#include <CuTest.h>
#include "codec/codecs.h"
#include "logger.h"

#ifndef testdatadir
#define testdatadir "data"
#endif

static int check_file_equals(FILE*, FILE*);

static void check_pbm_load(CuTest* tc, pbm_error_t err, const char* file, const char* msg) {
	LOG(info, "checking %s...", file);
	FILE* fp = fopen(file, "r");
	CuAssertPtrNotNullMsg(tc, msg, fp);
	if (!fp) {
		return;
	}
	pbm_info info;
	pbm_init(&info);
	pbm_error_t result = pbmcodec_pbm_read(&info, fp);
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

static void test_pbmcodec_pbm_read(CuTest* tc) {
#define CHECK_PBM_LOAD1(expected, file) check_pbm_load(tc, expected, file, "could not open " file)
#define CHECK_PBM_LOAD(expected, file) CHECK_PBM_LOAD1(expected, testdatadir "/" file)
	CHECK_PBM_LOAD(PBMCODEC_INVALID_SIGNATURE, "pbm/01_empty.pbm");
	CHECK_PBM_LOAD(PBMCODEC_INVALID_SIGNATURE, "pbm/01_invalid_sig1.pbm");
	CHECK_PBM_LOAD(PBMCODEC_INVALID_SIGNATURE, "pbm/01_invalid_sig2.pbm");
	CHECK_PBM_LOAD(PBMCODEC_INVALID_HEADER, "pbm/02_extra_header.pbm");
	CHECK_PBM_LOAD(PBMCODEC_INVALID_HEADER, "pbm/02_minus_size.pbm");
	CHECK_PBM_LOAD(PBMCODEC_INVALID_HEADER, "pbm/02_no_height.pbm");
	CHECK_PBM_LOAD(PBMCODEC_INVALID_HEADER, "pbm/02_no_width.pbm");
	CHECK_PBM_LOAD(PBM_SUCCESS, "pbm/03_extra_data.pbm");
	CHECK_PBM_LOAD(PBM_SUCCESS, "pbm/03_extra_row.pbm");
	CHECK_PBM_LOAD(PBMCODEC_INVALID_DATA, "pbm/03_lack_data.pbm");
	CHECK_PBM_LOAD(PBMCODEC_INVALID_DATA, "pbm/03_lack_row.pbm");
	CHECK_PBM_LOAD(PBMCODEC_INVALID_DATA, "pbm/03_invalid_data.pbm");
	CHECK_PBM_LOAD(PBM_SUCCESS, "pbm/04_crlf.pbm");
	CHECK_PBM_LOAD(PBM_SUCCESS, "pbm/04_lf.pbm");
}

static void test_pbmcodec_pbm_write(CuTest* tc) {
	pbm_info info;
	pbm_init(&info);
#ifdef _WIN32
	FILE* rfp = fopen(testdatadir "/pbm/04_crlf.pbm", "r");
#else
	FILE* rfp = fopen(testdatadir "/pbm/04_lf.pbm", "r");
#endif
	pbmcodec_pbm_read(&info, rfp);

#ifdef _WIN32
	FILE* wfp = fopen("pbm_test.write.tmp", "w+");
#else
	FILE* wfp = tmpfile();
#endif
	CuAssertPtrNotNull(tc, wfp);
	pbmcodec_pbm_write(&info, wfp);

	fseek(rfp, 0, SEEK_SET);
	fseek(wfp, 0, SEEK_SET);
	CuAssertIntEquals(tc, 1, check_file_equals(rfp, wfp));
	pbm_free(&info);
}

static int check_file_equals(FILE* afp, FILE* bfp) {
	int a, b;
	for (;;) {
		a = fgetc(afp);
		b = fgetc(bfp);
		if (a != b) {
			return 0;
		} else if (a == EOF) {
			return 1;
		}
	}
}

CuSuite* get_pbmcodec_test_suites() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_pbmcodec_pbm_read);
	SUITE_ADD_TEST(suite, test_pbmcodec_pbm_write);
	return suite;
}
