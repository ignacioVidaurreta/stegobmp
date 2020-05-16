#ifndef RC4_HEADER__H
#define RC4_HEADER__H

void swap(int vec[], int i, int j);
int* get_shuffled_vector(int* init_vec, const int key[], size_t key_len);
void print_vec(int vec[], int len);
int* generate_result(int* init_vec, int N, int* output_stream);
int* get_output_stream(int * output_stream, int N);
void rc4();

#endif