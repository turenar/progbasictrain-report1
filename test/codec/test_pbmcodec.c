#include "config.inc.h"

#include "test_pbmcodec.h"

CuSuite* get_test_suites() {
	CuSuite* suite = CuSuiteNew();
	CuSuiteAddSuite(suite, get_pbmcodec_test_suites());
	CuSuiteAddSuite(suite, get_pngcodec_test_suites());
	return suite;
}
