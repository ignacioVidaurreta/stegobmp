#include <stdio.h>
#include <openssl/evp.h> 
#include <openssl/ossl_typ.h>

#include "../include/errors.h"
#include "../include/encryption_decryption.h"
#include "../include/evp_encryption_decryption.h"

//TODO: handle errors
int epv_encrypt(unsigned char *stream, int stream_len, unsigned char *key,
            unsigned char *iv, unsigned char *cipherstream, cipher* evp_function){
    EVP_CIPHER_CTX *ctx;

    int len;

    int cipherstream_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        return FAILURE;

    // TODO: @MaruFuster preparar para distintos algoritmos y modos
    if(1 != EVP_EncryptInit_ex(ctx, evp_function(), NULL, key, iv))
        return FAILURE;

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, cipherstream, &len, stream, stream_len))
        return FAILURE;
    cipherstream_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, cipherstream + len, &len))
        return FAILURE;
    cipherstream_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return cipherstream_len;
}

int epv_decrypt(unsigned char *cipherstream, int cipherstream_len, unsigned char *key,
            unsigned char *iv, unsigned char *stream, cipher* evp_function){

    EVP_CIPHER_CTX *ctx;

    int len;

    int stream_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        return FAILURE;

    // TODO: @MaruFuster preparar para distintos algoritmos y modos
    if(1 != EVP_DecryptInit_ex(ctx, evp_function(), NULL, key, iv))
        return FAILURE;

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, stream, &len, cipherstream, cipherstream_len))
        return FAILURE;
    stream_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, stream + len, &len))
        return FAILURE;
    stream_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return stream_len;
}