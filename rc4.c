#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/rc4.h"
#include "include/cryptography.h"


#define N 256   // 2^8

void swap(unsigned char *a, unsigned char *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

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

int create_encoding_stream(pixel*** image, unsigned char *encoding_stream) {
    unsigned char key[6];
  
    get_key_from_image(image, key);
    int j = 0;

    for(int i = 0; i < N; i++)
        encoding_stream[i] = i;

    for(int i = 0; i < N; i++) {
        j = (j + encoding_stream[i] + key[i % 6]) % N;

        swap(&encoding_stream[i], &encoding_stream[j]);
    }

    return 0;
}

int apply(unsigned char *encoding_stream, char *stream, int len, unsigned char* response) {

    int i = 0;
    int j = 0;

    for(int n = 0; n < len; n++) {
        i = (i + 1) % N;
        j = (j + encoding_stream[i]) % N;

        swap(&encoding_stream[i], &encoding_stream[j]);
        int rnd = encoding_stream[(encoding_stream[i] + encoding_stream[j]) % N];

        response[n] = rnd ^ stream[n];

    }

    return 0;
}

unsigned char* RC4(pixel*** image, const unsigned char* stream, long len) {

    unsigned char encoding_stream[N];
    unsigned char* response = malloc(len * sizeof(unsigned char));
    create_encoding_stream(image, encoding_stream);

    apply(encoding_stream, (char *)stream, len, response);

    return response;
}
