#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/ossl_typ.h>

#include "../include/errors.h"
#include "../include/evp_encryption_decryption.h"

#define DES "des"
#define AES128 "aes128"
#define AES192 "aes192"
#define AES256 "aes256"

#define CBC "cbc"
#define ECB "ecb"
#define CFB "cfb"
#define OFB "ofb"

#define DES_i 0
#define AES128_i 1
#define AES192_i 2
#define AES256_i 3

#define CBC_i 0
#define ECB_i 1
#define CFB_i 2
#define OFB_i 3

static unsigned char* iv = (unsigned char*)"abcdabcdabcdabcdabcdabcdabcdabcd";

static cipher* ciphers[][4] = { \
    {EVP_des_cbc,     EVP_des_ecb,     EVP_des_cfb,     EVP_des_ofb},
    {EVP_aes_128_cbc, EVP_aes_128_ecb, EVP_aes_128_cfb, EVP_aes_128_ofb},
    {EVP_aes_192_cbc, EVP_aes_192_ecb, EVP_aes_192_cfb, EVP_aes_192_ofb},
    {EVP_aes_256_cbc, EVP_aes_256_ecb, EVP_aes_256_cfb, EVP_aes_256_ofb}};


/* Returns encrypted/decrypted stream depending on operation */
unsigned char* run_cipher_process(char* a, char* m, char* password, int operation, unsigned char* stream) {    
    int algorithm=-1, mode=-1;

    if(strcmp(DES, a) == 0)         algorithm = DES_i;
    else if(strcmp(AES128, a) == 0) algorithm = AES128_i;
    else if(strcmp(AES192, a) == 0) algorithm = AES192_i;
    else if(strcmp(AES256, a) == 0) algorithm = AES256_i;
    else 
        return stream;

    if(strcmp(CBC, m) == 0)      mode = CBC_i;
    else if(strcmp(ECB, m) == 0) mode = ECB_i;
    else if(strcmp(CFB, m) == 0) mode = CFB_i;
    else if(strcmp(OFB, m) == 0) mode = OFB_i;
    else 
        return stream;


    return stream;
}
