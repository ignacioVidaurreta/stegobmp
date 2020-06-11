#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"

#include "../include/cryptography.h"

void test_sha3_generates_hash(CuTest *tc){
    

    const char* password = "password";
    unsigned char * hash = compress_password(password);

    // CuAssertStrEquals(tc, expected_plaintext, decrypt(ciphertext, key_stream));

}

CuSuite* SymmetricCryptographySuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_sha3_generates_hash);
    return suite;
}
