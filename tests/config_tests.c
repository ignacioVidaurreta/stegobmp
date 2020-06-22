#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"
#include "../include/rc4.h"
#include "../include/lsbi.h"


void test_rc4_decodes_correctly(CuTest *tc){
    

    const unsigned char* expected_plaintext = "We attack at midnight";
    information* info = bmp_to_matrix("images/testImage.bmp");
    int* key = malloc(6*sizeof(int));
    
    char* ciphertext = RC4(info->matrix, expected_plaintext, strlen(expected_plaintext)+1);
    CuAssertStrEquals(tc, expected_plaintext, RC4(info->matrix, ciphertext, strlen(expected_plaintext)+1));
}


CuSuite* ConfigRC4Suite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_rc4_decodes_correctly);
    return suite;
}
