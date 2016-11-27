#include "config.inc.h"

#include "test_pbmcodec.h"
#include <stdio.h>

int main() {
	CuString* output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, get_pbmcodec_test_suites());
	CuSuiteAddSuite(suite, get_pngcodec_test_suites());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	return suite->failCount != 0;
}
