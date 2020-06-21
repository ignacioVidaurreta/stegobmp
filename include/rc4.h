#ifndef RC4_HEADER__H
#define RC4_HEADER__H

#include "../include/bmp.h"
#include <stdbool.h>

void swap(unsigned char vec[], int i, int j);
unsigned char* get_shuffled_vector(unsigned char* init_vec, const unsigned char key[], size_t key_len);
void print_vec(int vec[], int len);
unsigned char* generate_result(unsigned char* init_vec, int N, unsigned char* output_stream);
unsigned char* apply_xor(const unsigned char* initial_text, long len, unsigned char* cipher);
unsigned char* decrypt(const unsigned char* ciphertext, long len, unsigned char* cipher);
unsigned char* encrypt(const unsigned char* plaintext, long len, unsigned char* cipher);
const unsigned char* get_key_from_image(pixel*** image, unsigned char* key);
unsigned char* rc4(pixel*** image, const unsigned char* stream, long len, bool should_encrypt);
int get_enc_length(pixel*** image, const unsigned char* stream);
#endif
