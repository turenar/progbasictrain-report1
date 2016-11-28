#include "config.inc.h"

#include "test_pbmfilter.h"
#include <stdio.h>

int main() {
	CuString* output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, get_noise_filter_test_suites());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	fflush(stdout);
	return suite->failCount != 0;
}
