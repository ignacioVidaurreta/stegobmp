#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

#define BYTES_IN_256_BITS 32

unsigned char* compress_password(const char* password) {
    unsigned char* hash = malloc(BYTES_IN_256_BITS*sizeof(unsigned char));
    SHA256((unsigned char*)password, strlen(password), hash);
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
