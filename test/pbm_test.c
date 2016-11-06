#include <CuTest.h>

#include "pbm.h"

#ifndef testdatadir
#define testdatadir "data"
#endif

#define PBM_LOAD_FROM(file) pbm_load(&info, fopen(testdatadir "/" file, "r"))

static int check_file_equals(FILE*, FILE*);

static void test_pbm_load(CuTest* tc) {
	pbm_info info;
	CuAssertIntEquals(tc, PBM_INVALID_HEADER, PBM_LOAD_FROM("pbm/01_empty.pbm"));
	CuAssertIntEquals(tc, PBM_INVALID_HEADER, PBM_LOAD_FROM("pbm/01_invalid_sig1.pbm"));
	CuAssertIntEquals(tc, PBM_INVALID_HEADER, PBM_LOAD_FROM("pbm/01_invalid_sig2.pbm"));
	CuAssertIntEquals(tc, PBM_INVALID_HEADER, PBM_LOAD_FROM("pbm/02_extra_header.pbm"));
	CuAssertIntEquals(tc, PBM_INVALID_HEADER, PBM_LOAD_FROM("pbm/02_minus_size.pbm"));
	CuAssertIntEquals(tc, PBM_INVALID_HEADER, PBM_LOAD_FROM("pbm/02_no_height.pbm"));
	CuAssertIntEquals(tc, PBM_INVALID_HEADER, PBM_LOAD_FROM("pbm/02_no_width.pbm"));
	CuAssertIntEquals(tc, PBM_SUCCESS, PBM_LOAD_FROM("pbm/03_extra_data.pbm"));
	CuAssertIntEquals(tc, PBM_SUCCESS, PBM_LOAD_FROM("pbm/03_extra_row.pbm"));
	CuAssertIntEquals(tc, PBM_LACK_DATA, PBM_LOAD_FROM("pbm/03_lack_data.pbm"));
	CuAssertIntEquals(tc, PBM_LACK_DATA, PBM_LOAD_FROM("pbm/03_lack_row.pbm"));
	CuAssertIntEquals(tc, PBM_INVALID_DATA, PBM_LOAD_FROM("pbm/03_invalid_data.pbm"));
	CuAssertIntEquals(tc, PBM_SUCCESS, PBM_LOAD_FROM("pbm/04_crlf.pbm"));
	CuAssertIntEquals(tc, PBM_SUCCESS, PBM_LOAD_FROM("pbm/04_lf.pbm"));
}

static void test_pbm_write(CuTest* tc) {
	pbm_info info;
	FILE* rfp = fopen(testdatadir "/pbm/04_lf.pbm", "r");
	pbm_load(&info, rfp);

	FILE* wfp = tmpfile();
	pbm_write(&info, wfp);

	fseek(rfp, 0, SEEK_SET);
	fseek(wfp, 0, SEEK_SET);
	CuAssertIntEquals(tc, 1, check_file_equals(rfp, wfp));
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
	SUITE_ADD_TEST(suite, test_pbm_load);
	SUITE_ADD_TEST(suite, test_pbm_write);
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
