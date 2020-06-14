#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"
#include "../include/rc4.h"
#include "../include/lsbi.h"


void test_lsbi_decodes_correctly(CuTest *tc){
    
    const unsigned char* expected_plaintext = "We attack at midnight";
    size_t len = strlen(expected_plaintext);
    information* info = bmp_to_matrix("images/testImage.bmp");
    embed_lsbi(expected_plaintext, len, info->matrix, info->header->bmp_width, info->header->bmp_height);
    CuAssertStrEquals(tc, expected_plaintext, extract_lsbi(info->matrix, info->header->bmp_width, info->header->bmp_height));
    // information* info = bmp_to_matrix("images/testImage.bmp");
    // int* key = malloc(6*sizeof(int));
    
    // char* ciphertext = rc4(info->matrix, expected_plaintext, true);
    // 
}


CuSuite* ConfigLSBISuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_lsbi_decodes_correctly);
    return suite;
}
