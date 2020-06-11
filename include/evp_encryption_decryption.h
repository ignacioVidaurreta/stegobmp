#ifndef EVP_ENCRYPTION_DECRYPTION_H
#define EVP_ENCRYPTION_DECRYPTION_H

#include "encryption_decryption.h"
#include <openssl/ossl_typ.h>

typedef const EVP_CIPHER *cipher(void);

/*
In both cases, when we use them, we are responsible to provide a key an iv that corresponds to the
cryptographic function we are intending to use.
For example: if i'm using aes_cbc_256, I have to make sure i'm providing a 256 bits key and iv.
Neither of these functions are going to validate any of that.
*/

/* Returns legnth of cipherstream */
int epv_encrypt(unsigned char *stream, int stream_len, unsigned char *key,
            unsigned char *iv, unsigned char *cipherstream, const EVP_CIPHER * evp_function);

/* Returns length of decrypted stream */
int epv_decrypt(unsigned char *cipherstream, int cipherstream_len, unsigned char *key,
            unsigned char *iv, unsigned char *stream, const EVP_CIPHER * evp_function);


#endif /*   EVP_ENCRYPTION_DECRYPTION_H */