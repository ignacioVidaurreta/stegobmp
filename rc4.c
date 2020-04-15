#include <stdio.h>
#include <stdlib.h>
#include "include/rc4.h"

void swap(int vec[], int i, int j){
    int aux = vec[i];
    vec[i] = vec[j];
    vec[j] = aux;
}

int* get_shuffled_vector(int* init_vec, const int key[], size_t key_len){

    int j;
    for(int i = 0; i< 256; i++){
        j = (j + init_vec[i] + key[i % key_len]) % 256;
        swap(init_vec, i, j);
    }

    return init_vec;
}

void print_vec(int vec[], int len){
    printf("[ ");
    for(int i = 0; i< len; i++){
        printf("%d", vec[i]);
        if(i+1 != len){
            printf(", ");
        }
    }
    printf("]\n");
}

int* generate_result(int* init_vec, int N, int* output_stream){
    int i=0, j=0, k=0;

    while(k < N){
        i = (i+1) % 256;
        j = (j+ init_vec[i])% 256;
        swap(init_vec, i, j);
        
        output_stream[k++]= init_vec[(init_vec[i] + init_vec[j])% 256];
    }

    return output_stream;
}


int* get_output_stream(int * output_stream, int N){
    int* stream = malloc(256 * sizeof(int));

    for(int i = 0; i<256; i++){
        stream[i] = i;
    }

    const int secret_key[] = {2, 5, 2, 1, 9, 5,9, 1};
    
    size_t key_len = sizeof(secret_key)/sizeof(secret_key[0]);
    stream = get_shuffled_vector(stream, secret_key, key_len);
    printf("Initial shuffled vector: \n");
    print_vec(stream, 256);

    output_stream = generate_result(stream, N, output_stream);
    free(stream);

    return output_stream;
}

void rc4(){
    int N = 100;
    int* output_stream = malloc(N * sizeof(int));
    
    output_stream = get_output_stream(output_stream, N);

    printf("\n\nCipher output: \n");
    print_vec(output_stream, N);
    
    
    free(output_stream);
}