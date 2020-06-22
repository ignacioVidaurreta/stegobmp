#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "../include/steganography.h"
#include "../include/cryptography.h"
#include "../include/bmp.h"
#include "../include/lsbi.h"
#include "../include/print.h"
#include "../include/base.h"
#include "../include/logging.h"
#include "../include/errors.h"
#include "../include/rc4.h"

#define EXTENSION 5
#define STARTING_PIXEL 2
#define STARTING_BYTE 6

static int embed(const unsigned char* stream, int stream_size, pixel*** image, int width, int height, int hop) {
    
    /*
     * Log error if stream_size is too long to insert in image.
     * The longest that it will be able to fit is the number of
     * elements in the matrix width*height*components is in bytes
     */
    if((stream_size * BYTE) > (width * height * COMPONENTS)) {
        log_error_aux("Can't embed image: size exceeds available space");
        return ERROR_SIZE;
    }

    // array of bits that will be reused to calculate each byte to embed in bits
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);

    // i is index for stream of bytes
    // First two pixels are reserved for key
    int i = 0, x = STARTING_PIXEL, y = 0;
    pixel* pixel;

    int restart_point = STARTING_BYTE;
    for(int j = 0, shift  = STARTING_BYTE; j < stream_size*BYTE; j+=1) {
        // update the bits array with next byte
        if(j % BYTE == 0) {
            uchar_to_byte(bits, stream[i++]);
        }

        pixel = image[y][x];                    

        // which component do i have to embed in
        if(shift % COMPONENTS == 0) {
            pixel->blue = (pixel->blue & ~1) | (bits[j % BYTE]-CERO);
        }
        else if(shift % COMPONENTS == 1) {
            pixel->green = (pixel->green & ~1) | (bits[j % BYTE]-CERO);
        }
        else {
            pixel->red = (pixel->red & ~1) | (bits[j % BYTE]-CERO);
        }

        // TODO: This could be solved with modular arithmetics
        shift += hop;
        if (shift >= width*height*COMPONENTS){
            shift = ++restart_point; // If reached the end, start from the next avaiable byte.
        }
        // moving through the matrix of pixels    
        x = (shift/COMPONENTS) % width, y = ((shift/COMPONENTS) / width) % height;
    }
    free(bits);
    return SUCCESS;
}

// extracts data_size which resides in first 4 bytes of embeded stream (size of long)
// !!includes the key bits!!
long extract_data_size(pixel*** image, int width, int height, int hop, int* shift) {
    unsigned char bits[BYTE];
    unsigned char size_arr[DWORD];
    pixel* pixel;

    int i=0, x = STARTING_PIXEL, y = 0;
    int restart_point = STARTING_BYTE;
     // we will extract every embeded bit in the image
    for(int j = 0, shift=STARTING_BYTE; j < SIZE_OF_LONG_IN_BITS ; j++) {
        // update the stream array with last filled byte
        if(j % BYTE == 0 && j != 0) {
            size_arr[i++] = byte_to_uchar((const unsigned char*)bits);
        }

        pixel = image[y][x];
        
        if( shift % COMPONENTS == 0) {
            bits[j%BYTE] = (pixel->blue & 1) + CERO;
        }
        else if(shift % COMPONENTS == 1) {
            bits[j%BYTE] = (pixel->green & 1)  + CERO;
        }
        else {
            bits[j%BYTE] = (pixel->red & 1)  + CERO;
        }

        shift += hop;
        if (shift >= width*height*COMPONENTS){
            shift = ++restart_point; // If reached the end, start from the next avaiable byte.
        }
        // moving through the matrix of pixels    
        x = (shift/COMPONENTS) % width, y = ((shift/COMPONENTS) / width) % height;  
    }
    // update the stream array with its last byte
    size_arr[i++] = byte_to_uchar((const unsigned char*)bits);

    // print_array(size_arr,DWORD);
    
    unsigned char * decrypted = RC4(image,size_arr, DWORD);
    
    // printf("After RC4\n");
    // for(int i=0; i<4; i++) {
    //     printf("%d ", decrypted[i]);
    // }
    // printf("\n");
    // print_array(decrypted,DWORD);

    return get_len_from_stream(decrypted);
}


// extracts a stream of bytes from an image
static unsigned char* extract(pixel*** image, int width, int height, int hop, int is_encrypted) {
    int shift = 0;
    long data_size = extract_data_size(image, width, height, hop, &shift);
    int extension_size = EXTENSION;
    long stream_size = DWORD + data_size + extension_size;

    printf("extention_size = %d\n", extension_size);
    printf("data_size = %ld\n", data_size);
    printf("stream_size = %ld\n", stream_size);

    // array of bytes that will contain the stream
    unsigned char* stream = malloc(sizeof(*stream)*(stream_size));
    
    // array of bits that will be reused to extract bits and calculate each byte
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);

    // i is index for stream of bytes
    int i=0, x = STARTING_PIXEL, y = 0;
    pixel* pixel;
    int restart_point = STARTING_BYTE;
     // we will extract every embeded bit in the image
    for(int j = 0, shift=STARTING_BYTE; j < stream_size*BYTE ; j++) {
        // update the stream array with last filled byte
        if(j % BYTE == 0 && j != 0) {
            stream[i++] = byte_to_uchar((const unsigned char*)bits);
        }

        pixel = image[y][x];
        
        if( shift % COMPONENTS == 0) {
            bits[j%BYTE] = (pixel->blue & 1) + CERO;
        }
        else if(shift % COMPONENTS == 1) {
            bits[j%BYTE] = (pixel->green & 1)  + CERO;
        }
        else {
            bits[j%BYTE] = (pixel->red & 1)  + CERO;
        }

        shift += hop;
        if (shift >= width*height*COMPONENTS){
            shift = ++restart_point; // If reached the end, start from the next avaiable byte.
        }
        // moving through the matrix of pixels    
        x = (shift/COMPONENTS) % width, y = ((shift/COMPONENTS) / width) % height;
    }
    // update the stream array with its last byte
    stream[i++] = byte_to_uchar((const unsigned char*)bits);

    printf("\nended extract \n");
    // print_array(stream,stream_size);

    free(bits);    
    return stream;
}

static int get_hop_from_most_significant_bit(unsigned char pixel_component) {
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);
    uchar_to_byte(bits, pixel_component);
    return get_most_signifcant_bit(bits, BYTE);
}

int run_lsbi_embed(information* info, const unsigned char* stream, long stream_size) {
    printf("\nstarting embed\n");
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;
    pixel*** image = info->matrix;
    
    pixel* first_pixel = image[0][0];
    const int hop = get_hop_from_most_significant_bit(first_pixel->blue); 
    
    printf("HOP: %d\nblue component = %d\n", hop, first_pixel->blue);

    //process rc4
    const unsigned char* enc_stream = RC4(image, stream, stream_size);

    // print_array(enc_stream,stream_size);

    //embed
    embed(enc_stream, stream_size, image, width, height, hop);

    return SUCCESS;
}


unsigned char* run_lsbi_extract(information* info, int is_encrypted) {
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;
    pixel*** image = info->matrix;
    pixel* first_pixel = image[0][0];
    const int hop = get_hop_from_most_significant_bit(first_pixel->blue);  
    printf("HOP: %d\nblue component = %d\n", hop, first_pixel->blue);

    //extract
    unsigned char* extracted_stream = extract(image, width, height, hop, is_encrypted);
    
    //process rc4
    long data_size = extract_data_size(image, width, height, hop, 0);
    return RC4(image, (const unsigned char*) extracted_stream, DWORD + data_size + EXTENSION);
}