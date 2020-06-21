#ifndef RC4_HEADER__H
#define RC4_HEADER__H

#include "../include/bmp.h"
#include <stdbool.h>

void swap(int vec[], int i, int j);
int* get_shuffled_vector(int* init_vec, const int key[], size_t key_len);
void print_vec(int vec[], int len);
int* generate_result(int* init_vec, int N, int* output_stream);
unsigned char* apply_xor(const unsigned char* initial_text, int* cipher);
unsigned char* decrypt(const unsigned char* ciphertext, int* cipher);
unsigned char* encrypt(const unsigned char* plaintext, int* cipher);
const int* get_key_from_image(pixel*** image, int* key);
unsigned char* rc4(pixel*** image, const unsigned char* stream, bool should_encrypt);

#endif
