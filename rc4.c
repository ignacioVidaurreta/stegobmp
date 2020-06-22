#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/rc4.h"
#include "include/cryptography.h"

void swap(unsigned char vec[], int i, int j){
    unsigned char aux = vec[i];
    vec[i] = vec[j];
    vec[j] = aux;
}

unsigned char* get_shuffled_vector(unsigned char* base_stream, const unsigned char key[], size_t key_len){

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

// unsigned char* generate_result(unsigned char* base_stream, int N, unsigned char* key_stream){
//     int i=0, j=0, k=0;

//     while(k < N){
//         i = (i+1) % 256;
//         j = (j+ base_stream[i])% 256;
//         swap(base_stream, i, j);
        
//         key_stream[k++]= base_stream[(base_stream[i] + base_stream[j])% 256];
//     }

//     return key_stream;
// }

unsigned char* get_key_stream(pixel*** image, unsigned char * key_stream, int N){
    unsigned char* base_stream = malloc(256 * sizeof(int));

    for(int i = 0; i<256; i++){
        base_stream[i] = i;
    }
    unsigned char* key = malloc(6 * sizeof(unsigned char));
    get_key_from_image(image, key);
    
    base_stream = get_shuffled_vector(base_stream, key, 6);
    // printf("Initial shuffled vector: \n");
    // print_vec(stream, 256);

    return base_stream;
}

unsigned char* apply_xor(const unsigned char* initial_text, long len, unsigned char* key_stream){

    int i = 0, j = 0, k = 0;
    unsigned char* result = malloc(len * sizeof(char));
    while(k < len){
        i = (i+1) % 256;
        j = (j+ key_stream[i])% 256;
        swap(key_stream, i, j);
        
        int rand = key_stream[(key_stream[i] + key_stream[j])% 256];
        result[k] = initial_text[k] ^ rand;
        k++;
    }

    return result;
}

unsigned char* encrypt(const unsigned char* plaintext, long len, unsigned char* key_stream){
    return apply_xor(plaintext, len, key_stream);
}

unsigned char* decrypt(const unsigned char* ciphertext, long len, unsigned char* key_stream){
    return apply_xor(ciphertext, len, key_stream);
}

/*
 * For this method we assume that the bmp image will have more tan
 * 2 pixels per row which we feel is not a crazy assumption.
 */
const unsigned char* get_key_from_image(pixel*** image, unsigned char* key){
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
    
    // printf("\nin rc4 \n");
    // print_array(stream, len);

    unsigned char* key_stream = get_key_stream(image, stream, len);
    
    if(should_encrypt){
      return encrypt(stream, len, key_stream);
    }
   
    return decrypt(stream, len, key_stream);
}
