#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/ossl_typ.h>

#include "../include/symmetric_cryptography.h"
#include "../include/errors.h"

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

static unsigned char* iv = {(unsigned char)10,(unsigned char)20,(unsigned char)30,(unsigned char)40,
(unsigned char)50,(unsigned char)60,(unsigned char)70,(unsigned char)80};

// TODO: esto deberia retornar otra cosa
int run_encryption(char* a, char* m, char* password) {    
    int algorithm=-1, mode=-1;

    if(strcmp(DES, a) == 0)         algorithm = DES_i;
    else if(strcmp(AES128, a) == 0) algorithm = AES128_i;
    else if(strcmp(AES192, a) == 0) algorithm = AES192_i;
    else if(strcmp(AES256, a) == 0) algorithm = AES256_i;
    else 
        return PARAM_ERROR;

    if(strcmp(CBC, m) == 0)      mode = CBC_i;
    else if(strcmp(ECB, m) == 0) mode = ECB_i;
    else if(strcmp(CFB, m) == 0) mode = CFB_i;
    else if(strcmp(OFB, m) == 0) mode = OFB_i;
    else 
        return PARAM_ERROR;


    return SUCCESS;
}

// int main(int argc, char * argv[]){

//     char* message = "hola que tal amigos";
//     printf("Original message: %s\n", message);

//     long len = (long)strlen(message);
//     unsigned char* encrypted_stream = des_cbc_encrypt("pass", (unsigned char*)message, len);
//     // unsigned char* decrypted_stream = des_cbc_decrypt("pass", encrypted_stream);
    
//     // char* decrypted_message = (char*)decrypted_stream;
//     // printf("Decrypted message: %s\n", decrypted_message);
//     // free(encrypted_stream);
//     // free(decrypted_stream);
//     // return 0;
// }
