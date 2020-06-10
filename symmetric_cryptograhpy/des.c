#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/sha.h>

#include "../include/errors.h"

#define TRUE 1
#define FALSE 0
#define DWORD 4
#define DES_BLOCK_SIZE 8

/*
DES ENCRYPTION
DES_cblock is an 8 unsigned char vector
In order to encrypt, we need to do so giving to the function 8 bytes per time
*/

// TODO: esto no deberia ser exlusivo de DES
// size should be lower or equal to 160 bits
unsigned char* compress_password(const char* password, size_t size) {
    unsigned char* hash = malloc(20*sizeof(unsigned char));
    SHA1(password, strlen(password), hash);
    return hash; 
}

void append_len_to_stream(unsigned char* stream, int len) {
    stream[0] = (len >> 24) & 0xFF;
    stream[1] = (len >> 16) & 0xFF;
    stream[2] = (len >> 8) & 0xFF;
    stream[3] = len & 0xFF;
}

long get_len_from_stream(unsigned char* stream) {
    long len = (int)stream[3] | ( (int)stream[2] << 8 ) | ( (int)stream[1] << 16 ) | ( (int)stream[0] << 24 );;
    return len;
}

DES_key_schedule* get_key_schedule(char* password) {
    DES_key_schedule* ks = malloc(sizeof(*ks));
    if(ks == NULL) return ks;
    unsigned char* key = compress_password(password, DES_BLOCK_SIZE);  // we need to fit our password in a DES BLOCK

    // TODO: estaria bueno ver como aumentar la seguridad de la clave
    DES_set_key_unchecked((const_DES_cblock*)key, ks); // TODO: posible punto de error por size of key
}

/* Returns: size of encrypted message || encrypted(stream) */
unsigned char* des_cbc_encrypt(char* password, unsigned char* stream, long stream_length) {

    printf("Starting encryption...\n");

    int needed_padding = DES_BLOCK_SIZE - stream_length%DES_BLOCK_SIZE;      // how many bytes do I need
    int complete_blocks = stream_length/DES_BLOCK_SIZE;     // qty of complete blocks
    long encrypted_size = stream_length + needed_padding;

    printf("Original length: %ld, Needed padding: %d\nComplete blocks: %d\nEncrypted size: %ld\n",
            stream_length, needed_padding, complete_blocks, encrypted_size);

    unsigned char* encrypted_stream = malloc(sizeof(char)*DWORD + encrypted_size);
    if(encrypted_stream == NULL) {
        return encrypted_stream;
    }

    DES_key_schedule* ks = get_key_schedule(password);
    if(ks == NULL) {
        free(encrypted_stream);
        return NULL;
    }

    printf("Key already set\n");

    for(int i=0; i<complete_blocks; i++) {
        // we encrypt each block and store it in our stream
        DES_ecb_encrypt((const_DES_cblock*)(stream+(i*DES_BLOCK_SIZE)),
                             (DES_cblock*)(encrypted_stream+DWORD+(i*DES_BLOCK_SIZE)), ks, DES_ENCRYPT);
    }

    printf("Complete blocks done\n");

    if(needed_padding > 0) {
    
        printf("Starting padding\n");
    
        unsigned char* last_block = malloc(DES_BLOCK_SIZE*sizeof(unsigned char));
        if(last_block == NULL) {
            free(encrypted_stream);
            free(ks);
            return NULL;
        }
        strncpy(last_block, stream+(DES_BLOCK_SIZE*complete_blocks), DES_BLOCK_SIZE-needed_padding);
        for(int i=0; i<needed_padding; i++){
            last_block[DES_BLOCK_SIZE - needed_padding + i] = (unsigned char) rand();
        }
        DES_ecb_encrypt((const_DES_cblock*)last_block, (DES_cblock*)(encrypted_stream+DWORD+(complete_blocks*DES_BLOCK_SIZE)), ks, DES_ENCRYPT);
        free(last_block); 
    }
    
    printf("Finished encryption\n");
    
    append_len_to_stream(encrypted_stream, encrypted_size); // we store the encrypted length in our stream
    
    printf("Finished concatenation\n");
    free(ks);
    return encrypted_stream;
}

unsigned char* des_cbc_decrypt(char* password, unsigned char* encrypted_stream) {
    
    printf("Starting decryption...\n");

    long len = get_len_from_stream(encrypted_stream);
    int blocks = len/DES_BLOCK_SIZE;
    unsigned char* start = encrypted_stream + DWORD;
    
    printf("Length: %ld\nBlocks: %d\n",len, blocks);

    unsigned char* decrypted_stream = malloc(sizeof(unsigned char)*len);
    if(decrypted_stream == NULL) return NULL;

    DES_key_schedule* ks = get_key_schedule(password);
    if(ks == NULL) {
        free(decrypted_stream);
        return NULL;
    }

    printf("Key already set\n");
    
    for(int i=0; i<blocks; i++) {
        // we decrypt each block and store it in our stream
        DES_ecb_encrypt((const_DES_cblock*)(start+(i*DES_BLOCK_SIZE)),
                             (DES_cblock*)(decrypted_stream+(i*DES_BLOCK_SIZE)), ks, DES_DECRYPT);
    }

    printf("Decryption completed\n");

    free(ks);
    return decrypted_stream;
}
