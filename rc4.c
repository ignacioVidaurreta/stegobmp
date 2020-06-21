#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/rc4.h"

void swap(int vec[], int i, int j){
    int aux = vec[i];
    vec[i] = vec[j];
    vec[j] = aux;
}

int* get_shuffled_vector(int* base_stream, const int key[], size_t key_len){

    int j = 0;
    for(int i = 0; i< 256; i++){
        j = (j + base_stream[i] + key[i % key_len]) % 256;
        swap(base_stream, i, j);
    }

    return base_stream;
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

int* generate_result(int* base_stream, int N, int* key_stream){
    int i=0, j=0, k=0;

    while(k < N){
        i = (i+1) % 256;
        j = (j+ base_stream[i])% 256;
        swap(base_stream, i, j);
        
        key_stream[k++]= base_stream[(base_stream[i] + base_stream[j])% 256];
    }

    return key_stream;
}

int* get_key_stream(int * key_stream, int N){
    int* base_stream = malloc(256 * sizeof(int));

    for(int i = 0; i<256; i++){
        base_stream[i] = i;
    }

    const int secret_key[] = {2, 5, 2, 1, 9, 5};
    
    size_t key_len = sizeof(secret_key)/sizeof(secret_key[0]);
    base_stream = get_shuffled_vector(base_stream, secret_key, key_len);
    // printf("Initial shuffled vector: \n");
    // print_vec(stream, 256);

    key_stream = generate_result(base_stream, N, key_stream);
    free(base_stream);

    return key_stream;
}

unsigned char* apply_xor(const unsigned char* initial_text, long len, int* key_stream){

    unsigned char* result = malloc(len * sizeof(char));
    for(int i = 0; i<len; i++){
        result[i] = initial_text[i] ^ key_stream[i % 6];
        //printf("%d        i =  %d     restult[i] = %d\n", initial_text[i], i, result[i]);
    }
    result[len] = '\0';


    return result;
}

unsigned char* encrypt(const unsigned char* plaintext, long len, int* key_stream){
    return apply_xor(plaintext, len, key_stream);
}

unsigned char* decrypt(const unsigned char* ciphertext, long len, int* key_stream){
    return apply_xor(ciphertext, len, key_stream);
}

/*
 * For this method we assume that the bmp image will have more tan
 * 2 pixels per row which we feel is not a crazy assumption.
 */
const int* get_key_from_image(pixel*** image, int* key){
    pixel* first_pixel = image[0][0];
    pixel* second_pixel = image[0][1];

    //order: Blue - Green - Red
    key[0] = first_pixel->blue;
    key[1] = first_pixel->green;
    key[2] = first_pixel->red;
    key[3] = second_pixel->blue;
    key[4] = second_pixel->green;
    key[5] = second_pixel->red;
   

    return key;
}

unsigned char* rc4(pixel*** image, const unsigned char* stream, long len, bool should_encrypt){
    
    printf("\nin rc4 \n");
    print_array(stream, len);

    int* key = malloc(6 * sizeof(int));
    get_key_from_image(image, key);
    
    if(should_encrypt){
      return encrypt(stream, len, key);
    }
   
    return decrypt(stream, len, key);
}
