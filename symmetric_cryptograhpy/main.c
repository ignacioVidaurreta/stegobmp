#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "all.h"
#include "../include/errors.h"

#define DES "des"
#define AES128 "aes128"
#define AES192 "aes192"
#define AES256 "aes256"

// TODO: si o si se necesita la password, si ese dato no esta no se deberia
// ni llamar a esta funcion y solo se esteganografia

// TODO: esto deberia retornar otra cosa
int run_encryption(char* algorithm, char mode, char* password) {
    if(strcmp(DES, algorithm) == 0) {
        // call des
    }
    else if(strcmp(AES128, algorithm) == 0) {
        // call aes 128
    }
    else if(strcmp(AES192, algorithm) == 0) {
        // call aes 192
    }
    else if(strcmp(AES256, algorithm) == 0) {
        // call aes 256
    }
    else {
        return PARAM_ERROR;
    }
    return SUCCESS;
}

int main(int argc, char * argv[]){

    char* message = "hola que tal amigos";
    printf("Original message: %s\n", message);

    long len = (long)strlen(message);
    unsigned char* encrypted_stream = des_cbc_encrypt("pass", (unsigned char*)message, len);
    unsigned char* decrypted_stream = des_cbc_decrypt("pass", encrypted_stream);
    
    char* decrypted_message = (char*)decrypted_stream;
    printf("Decrypted message: %s\n", decrypted_message);
    return 0;
}