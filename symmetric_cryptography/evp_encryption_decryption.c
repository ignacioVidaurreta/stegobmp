#include <stdio.h>
#include <openssl/ossl_typ.h>

int encrypt(unsigned char *stream, int stream_len, unsigned char *key,
            unsigned char *iv, unsigned char *cipherstream){
    EVP_CIPHER_CTX *ctx;

    int len;

    int cipherstream_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    // TODO: @MaruFuster preparar para distintos algoritmos y modos
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, cipherstream, &len, stream, stream_len))
        handleErrors();
    cipherstream_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, cipherstream + len, &len))
        handleErrors();
    cipherstream_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return cipherstream_len;
}

int decrypt(unsigned char *cipherstream, int cipherstream_len, unsigned char *key,
            unsigned char *iv, unsigned char *stream){

    EVP_CIPHER_CTX *ctx;

    int len;

    int stream_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    // TODO: @MaruFuster preparar para distintos algoritmos y modos
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, stream, &len, cipherstream, cipherstream_len))
        handleErrors();
    stream_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, stream + len, &len))
        handleErrors();
    stream_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return stream_len;
}