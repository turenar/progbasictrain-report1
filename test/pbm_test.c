#include "config.inc.h"

#include <CuTest.h>
#include "codec/codecs.h"

#ifndef testdatadir
#define testdatadir "data"
#endif

static int check_file_equals(FILE*, FILE*);

static void test_pbmcodec_pbm_read(CuTest* tc) {
#define CHECK_PBM_LOAD1(expected, file) { \
    FILE* fp = fopen(file, "r"); \
        if(!fp){CuFail(tc,"Could not load: " file);} \
        else{ CuAssertIntEquals(tc, expected, pbmcodec_pbm_read(&info, fp));}}
#define CHECK_PBM_LOAD(expected, file) CHECK_PBM_LOAD1(expected, testdatadir "/" file)
	pbm_info info;
	pbm_init(&info);
	CHECK_PBM_LOAD(PBMCODEC_INVALID_SIGNATURE, "pbm/01_empty.pbm")
	CHECK_PBM_LOAD(PBMCODEC_INVALID_SIGNATURE, "pbm/01_invalid_sig1.pbm")
	CHECK_PBM_LOAD(PBMCODEC_INVALID_SIGNATURE, "pbm/01_invalid_sig2.pbm")
	CHECK_PBM_LOAD(PBMCODEC_INVALID_HEADER, "pbm/02_extra_header.pbm")
	CHECK_PBM_LOAD(PBMCODEC_INVALID_HEADER, "pbm/02_minus_size.pbm")
	CHECK_PBM_LOAD(PBMCODEC_INVALID_HEADER, "pbm/02_no_height.pbm")
	CHECK_PBM_LOAD(PBMCODEC_INVALID_HEADER, "pbm/02_no_width.pbm")
	CHECK_PBM_LOAD(PBM_SUCCESS, "pbm/03_extra_data.pbm")
	CHECK_PBM_LOAD(PBM_SUCCESS, "pbm/03_extra_row.pbm")
	CHECK_PBM_LOAD(PBMCODEC_INVALID_DATA, "pbm/03_lack_data.pbm")
	CHECK_PBM_LOAD(PBMCODEC_INVALID_DATA, "pbm/03_lack_row.pbm")
	CHECK_PBM_LOAD(PBMCODEC_INVALID_DATA, "pbm/03_invalid_data.pbm")
	CHECK_PBM_LOAD(PBM_SUCCESS, "pbm/04_crlf.pbm")
	CHECK_PBM_LOAD(PBM_SUCCESS, "pbm/04_lf.pbm")
	pbm_free(&info);
}

static void test_pbmcodec_pbm_write(CuTest* tc) {
	pbm_info info;
	pbm_init(&info);
#ifdef __WIN32__
	FILE* rfp = fopen(testdatadir "/pbm/04_crlf.pbm", "r");
#else
	FILE* rfp = fopen(testdatadir "/pbm/04_lf.pbm", "r");
#endif
	pbmcodec_pbm_read(&info, rfp);

#ifdef __WIN32__
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

static void test_pbm_free(CuTest* tc) {
	pbm_info info;
	pbm_init(&info);
	pbm_free(&info);
	CuAssertPtrEquals(tc, NULL, info.data);

	FILE* rfp = fopen(testdatadir "/pbm/04_lf.pbm", "r");
	pbmcodec_pbm_read(&info, rfp);
	pbm_free(&info);
	CuAssertPtrEquals(tc, NULL, info.data);
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

static CuSuite* get_pbm_test_suites() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_pbmcodec_pbm_read);
	SUITE_ADD_TEST(suite, test_pbmcodec_pbm_write);
	SUITE_ADD_TEST(suite, test_pbm_free);
	return suite;
}

int main() {
	CuString* output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, get_pbm_test_suites());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	return suite->failCount != 0;
}
