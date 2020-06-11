#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"
#include "../include/symmetric_cryptography.h"


void test_des_ecb_encrypts_decrypts(CuTest *tc){

    char* message = "hola que tal amigos";
    printf("Original message: %s\n", message);
    
    long len = (long)(strlen(message) + 1);
    
    unsigned char* encrypted_stream = des_ecb_encrypt("pass", (unsigned char*)message, len);

    CuAssertStrEquals(tc, message, (char* )des_ecb_decrypt("pass", encrypted_stream));
}

void test_des_cbc_encrypts_decrypts(CuTest *tc){

    char* message = "hola que tal amigos";
    printf("Original message: %s\n", message);
    
    long len = (long)(strlen(message) + 1);
    
    unsigned char* encrypted_stream = des_cbc_encrypt("pass", (unsigned char*)message, len);

    CuAssertStrEquals(tc, message, (char* )des_cbc_decrypt("pass", encrypted_stream));
}


CuSuite* SymmetricCryptographySuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_des_ecb_encrypts_decrypts);
    SUITE_ADD_TEST(suite, test_des_cbc_encrypts_decrypts);
    return suite;
}
