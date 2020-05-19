#include <stdio.h>
#include "CuTest.h"

CuSuite* ConfigRC4Suite();


void RunAllTests(void){
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, ConfigRC4Suite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);

}

int main(void) {
    RunAllTests();
}