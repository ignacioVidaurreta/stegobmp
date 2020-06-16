#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"

#include "../include/parser.h"
#include "../include/cryptography.h"
#include "../include/encryption_decryption.h"

#define TRUE 1
#define FALSE 0

typedef struct test_struct {
    char* password;
    unsigned char* stream;
    int stream_len;
    enc_alg algorithm;
    chain_mode mode;
}test_struct;

test_struct* initialize_parameters(enc_alg algorithm, chain_mode mode) {
    test_struct* parameters = malloc(sizeof(*parameters));
    parameters->password = "password";
    parameters->stream = (unsigned char*)"test stream";
    parameters->stream_len = strlen(parameters->stream)+1;
    parameters->algorithm = algorithm;
    parameters->mode = mode;
    return parameters;
}

void test_very_short_password(CuTest *tc){
    
    test_struct* p = initialize_parameters(DES, CBC);
    p->password = "p";
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);

    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_very_long_password(CuTest *tc){
    
    test_struct* p = initialize_parameters(DES, CBC);
    p->password = "this is an extremely long password to check if the cipher process can compress this properly";
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_len_properly_appended(CuTest *tc){
    
    test_struct* p = initialize_parameters(DES, CBC);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, TRUE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, (enc_info->output_stream)+4, enc_info->output_len, FALSE);
    int len = get_len_from_stream(enc_info->output_stream);

    CuAssertIntEquals(tc, enc_info->output_len, len);
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_des_cbc(CuTest *tc){
    test_struct* p = initialize_parameters(DES, CBC);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_des_ecb(CuTest *tc){
    test_struct* p = initialize_parameters(DES, ECB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_des_cfb(CuTest *tc){
    test_struct* p = initialize_parameters(DES, CFB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_des_ofb(CuTest *tc){
    test_struct* p = initialize_parameters(DES, OFB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_128_cbc(CuTest *tc){
    test_struct* p = initialize_parameters(AES128, CBC);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_128_ecb(CuTest *tc){
    test_struct* p = initialize_parameters(AES128, ECB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_128_cfb(CuTest *tc){
    test_struct* p = initialize_parameters(AES128, CFB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_128_ofb(CuTest *tc){
    test_struct* p = initialize_parameters(AES128, OFB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_192_cbc(CuTest *tc){
    test_struct* p = initialize_parameters(AES192, CBC);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_192_ecb(CuTest *tc){
    test_struct* p = initialize_parameters(AES192, ECB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_192_cfb(CuTest *tc){
    test_struct* p = initialize_parameters(AES192, CFB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_192_ofb(CuTest *tc){
    test_struct* p = initialize_parameters(AES192, OFB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_256_cbc(CuTest *tc){
    test_struct* p = initialize_parameters(AES256, CBC);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_256_ecb(CuTest *tc){
    test_struct* p = initialize_parameters(AES256, ECB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_256_cfb(CuTest *tc){
    test_struct* p = initialize_parameters(AES256, CFB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

void test_aes_256_ofb(CuTest *tc){
    test_struct* p = initialize_parameters(AES256, OFB);
    
    int operation = ENCRYPT;
    cipher_info* enc_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, p->stream, p->stream_len, FALSE);
    
    operation = DECRYPT;
    cipher_info* dec_info = run_cipher_process(p->algorithm, p->mode, p->password, operation, enc_info->output_stream, enc_info->output_len, FALSE);
    
    CuAssertStrEquals(tc, p->stream, dec_info->output_stream);
}

CuSuite* SymmetricCryptographySuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_very_short_password);
    SUITE_ADD_TEST(suite, test_very_long_password);
    SUITE_ADD_TEST(suite, test_len_properly_appended);
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
