#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

// size should be lower or equal to 160 bits
unsigned char* compress_password(const char* password) {
    unsigned char* hash = malloc(20*sizeof(unsigned char));
    SHA1((unsigned char*)password, strlen(password), hash);
    return hash; 
}

unsigned char* get_iv(int bytes) {
    unsigned char* iv = malloc(sizeof(unsigned char)*bytes);
    for(int i=0; i<bytes; i++) {
        iv[i] = (unsigned char) rand();
    }
    return iv;
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
