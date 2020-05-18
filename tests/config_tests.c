#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"
#include "../include/rc4.h"


void test_rc4_decodes_correctly(CuTest *tc){
    

    const char* expected_plaintext = "We attack at midnight";
    int N = 100;
    int* key_stream = malloc(N * sizeof(int));

    key_stream = get_key_stream(key_stream, N);
    
    char * ciphertext = encrypt(expected_plaintext, key_stream);

    CuAssertStrEquals(tc, expected_plaintext, decrypt(ciphertext, key_stream));

}


CuSuite* ConfigRC4Suite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_rc4_decodes_correctly);
    return suite;
}
