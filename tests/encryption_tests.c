#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"

#include "../include/cryptography.h"
#include "../include/encryption_decryption.h"

typedef struct test_struct {
    char* password;
    unsigned char* stream;
    int stream_len;
    char* algorithm;
    char* mode;
}test_struct;

test_struct* initialize_parameters(char* algorithm, char* mode) {
    test_struct* parameters = malloc(sizeof(*parameters));
    parameters->password = "password";
    parameters->stream = (unsigned char*)"test stream";
    parameters->stream_len = strlen(parameters->stream)+1;
    parameters->algorithm = algorithm;
    parameters->mode = mode;
    return parameters;
}

void test_upper_case_will_return_null(CuTest *tc){
    test_struct* p = initialize_parameters("DES", "cbc");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, NULL, decrypted);
}

void test_des_cbc(CuTest *tc){
    test_struct* p = initialize_parameters("des", "cbc");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_des_ecb(CuTest *tc){
    test_struct* p = initialize_parameters("des", "ecb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_des_cfb(CuTest *tc){
    test_struct* p = initialize_parameters("des", "cfb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_des_ofb(CuTest *tc){
    test_struct* p = initialize_parameters("des", "ofb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_128_cbc(CuTest *tc){
    test_struct* p = initialize_parameters("aes128", "cbc");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_128_ecb(CuTest *tc){
    test_struct* p = initialize_parameters("aes128", "ecb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_128_cfb(CuTest *tc){
    test_struct* p = initialize_parameters("aes128", "cfb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_128_ofb(CuTest *tc){
    test_struct* p = initialize_parameters("aes128", "ofb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_192_cbc(CuTest *tc){
    test_struct* p = initialize_parameters("aes192", "cbc");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_192_ecb(CuTest *tc){
    test_struct* p = initialize_parameters("aes192", "ecb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_192_cfb(CuTest *tc){
    test_struct* p = initialize_parameters("aes192", "cfb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_192_ofb(CuTest *tc){
    test_struct* p = initialize_parameters("aes192", "ofb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_256_cbc(CuTest *tc){
    test_struct* p = initialize_parameters("aes256", "cbc");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_256_ecb(CuTest *tc){
    test_struct* p = initialize_parameters("aes256", "ecb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_256_cfb(CuTest *tc){
    test_struct* p = initialize_parameters("aes256", "cfb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

void test_aes_256_ofb(CuTest *tc){
    test_struct* p = initialize_parameters("aes256", "ofb");
    
    int operation = ENCRYPT;
    unsigned char* encrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    operation = DECRYPT;
    unsigned char* decrypted = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len);
    
    CuAssertStrEquals(tc, p->stream, decrypted);
}

CuSuite* SymmetricCryptographySuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_upper_case_will_return_null);
    SUITE_ADD_TEST(suite, test_des_cbc);
    SUITE_ADD_TEST(suite, test_des_ecb);
    SUITE_ADD_TEST(suite, test_des_cfb);
    SUITE_ADD_TEST(suite, test_des_ofb);
    SUITE_ADD_TEST(suite, test_aes_128_cbc);
    SUITE_ADD_TEST(suite, test_aes_128_ecb);
    SUITE_ADD_TEST(suite, test_aes_128_cfb);
    SUITE_ADD_TEST(suite, test_aes_128_ofb);
    SUITE_ADD_TEST(suite, test_aes_192_cbc);
    SUITE_ADD_TEST(suite, test_aes_192_ecb);
    SUITE_ADD_TEST(suite, test_aes_192_cfb);
    SUITE_ADD_TEST(suite, test_aes_192_ofb);
    SUITE_ADD_TEST(suite, test_aes_256_cbc);
    SUITE_ADD_TEST(suite, test_aes_256_ecb);
    SUITE_ADD_TEST(suite, test_aes_256_cfb);
    SUITE_ADD_TEST(suite, test_aes_256_ofb);
    return suite;
}
