#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/ossl_typ.h>

#include "../include/cryptography.h"
#include "../include/encryption_decryption.h"
#include "../include/errors.h"
#include "../include/evp_encryption_decryption.h"
#include "../include/logging.h"
#include "../include/parser.h"

#define BLOCK_SIZE 8
#define DWORD 4

#define FALSE 0

static cipher* ciphers[][4] = { \
    {EVP_aes_128_ecb, EVP_aes_128_cfb, EVP_aes_128_ofb, EVP_aes_128_cbc},
    {EVP_aes_192_ecb, EVP_aes_192_cfb, EVP_aes_192_ofb, EVP_aes_192_cbc},
    {EVP_aes_256_ecb, EVP_aes_256_cfb, EVP_aes_256_ofb, EVP_aes_256_cbc},
    {EVP_des_ecb,     EVP_des_cfb,     EVP_des_ofb,     EVP_des_cbc}};


/* Returns encrypted/decrypted stream depending on operation + len of corresponding stream */
cipher_info* run_cipher_process(enc_alg algorithm, chain_mode mode, char* password,
    int operation, unsigned char* stream, int stream_len, int append_len) {    

    // printf("Starting cipher process\n");

    // printf("Chose algorithm %d and mode %d\n",algorithm, mode);

    const EVP_CIPHER *cipher;
    cipher = ciphers[algorithm][mode]();
    unsigned char key[EVP_CIPHER_key_length(cipher)];
    unsigned char iv[EVP_CIPHER_iv_length(cipher)];

    const EVP_MD *dgst = NULL;
    const unsigned char *salt = NULL;

    dgst = EVP_sha256();

    // printf("Parameters set\n");

    EVP_BytesToKey(cipher, dgst, salt, (unsigned char*)password, strlen(password),1, key, iv);

    // printf("Finished to get IV and key\n");

    // TODO: check if 2 is enough or too much
    unsigned char* output_stream = malloc(sizeof(unsigned char)*(2*stream_len));
    if(output_stream == NULL) {
        log_error_aux("Memory allocation failed on run_cipher_process (output_stream)");
        return NULL;
    }

    int output_len;
    if(operation == ENCRYPT) {
        int offset = append_len == FALSE ? 0 : DWORD;
        output_len = epv_encrypt(stream, stream_len, key, iv, output_stream + offset, ciphers[algorithm][mode]);
        if(offset) {
            append_len_to_stream(output_stream, output_len);
        }
    }
    else if(operation == DECRYPT) {
        output_len = epv_decrypt(stream, stream_len, key, iv, output_stream, ciphers[algorithm][mode]);
    }

    cipher_info* info = malloc(sizeof(*info));
    if(info == NULL) {
        free(output_stream);
        log_error_aux("Memory allocation failed on run_cipher_process (info)");
        return NULL;
    }

    info->output_len = output_len;
    info->output_stream = output_stream;

    // printf("Finishing cipher process\n");

    return info;
}