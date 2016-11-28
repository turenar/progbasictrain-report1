#include "config.inc.h"

#include "test_pbmcore.h"
#include <stdio.h>

CuSuite* get_test_suites() {
	CuSuite* suite = CuSuiteNew();
	CuSuiteAddSuite(suite, get_pbm_test_suites());
	return suite;
}
