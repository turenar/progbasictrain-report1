#include "config.inc.h"

#include "test_pbmfilter.h"

CuSuite* get_test_suites() {
	CuSuite* suite = CuSuiteNew();
	CuSuiteAddSuite(suite, get_noise_filter_test_suites());
	CuSuiteAddSuite(suite, get_diff_filter_test_suites());
	return suite;
}
