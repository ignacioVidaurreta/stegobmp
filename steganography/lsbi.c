#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "../include/rc4.h"
#include "../include/steganography.h"
#include "../include/bmp.h"
#include "../include/lsb1.h"
#include "../include/print.h"
#include "../include/base.h"
#include "../include/logging.h" 
#include "../include/errors.h"
#include "../include/rc4.h"
#include "../include/string.h"
#include "../include/steganography.h"

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
    int i = 0, x = 2, y = height-1;
    pixel* pixel;

    int restart_point = x;
    for(int j = 0, shift  = 0; j < stream_size*BYTE; j+=1) {
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
        x = (shift/COMPONENTS) % width, y = height-1-((shift/COMPONENTS) / width);
    }
    free(bits);
    return SUCCESS;
}

// extracts data_size which resides in first 4 bytes of embeded stream (size of long)
// !!includes the key bits!!.
long extract_data_size(pixel*** image, int width, int height, int hop, int* shift) {
    long size = 0;
    int x,y;
    x = (*shift/COMPONENTS) % width;
    int restart_point = x;
    y = height-1-((*shift/COMPONENTS) / width);
    pixel* pixel;

    // we need to get first 32 bits
    for(int j = 0; j <  SIZE_OF_LONG_IN_BITS ; j++) {
        pixel = image[y][x];
       
        if( *shift % COMPONENTS == 0) {
            size += (pixel->blue & 1) * pow(2,SIZE_OF_LONG_IN_BITS-j-1);
        }
        else if(*shift % COMPONENTS == 1) {
            size += (pixel->green & 1) * pow(2,SIZE_OF_LONG_IN_BITS-j-1);
        }
        else {
            size += (pixel->red & 1) * pow(2,SIZE_OF_LONG_IN_BITS-j-1);
        }

        // TODO: This could be solved with modular arithmetics
        *shift += hop;
        if (*shift >= width*height*COMPONENTS){
            *shift = ++restart_point; // If reached the end, start from the next avaiable byte.
        }
        // moving through the matrix of pixels    
        x = (*shift/COMPONENTS) % width, y = height-1-((*shift/COMPONENTS) / width);
    }
    return size;
}


/*
 * Calculates extension_size which resides after data_size and data in stream.
 * We read from shift (where '.' should be) until we find '\0'
 * and we convert every 8 bits to a byte to check so
 */
int calculate_extension_size(pixel*** image, int width, int height, long data_size, int hop, int shift) {
    int size = 0, i =0;
    // Advance to the end of the data. We need to transform it to bits since
    // we are doing bitwise operations here
    shift+=hop*data_size*BYTE; 
    int x = (shift/COMPONENTS) % width, y = height-1-((shift/COMPONENTS) / width);

    unsigned char* data = malloc(sizeof(*data)*(BLOCK_FOR_EXTENSION_SIZE*100));
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);

    pixel* pixel;
    for(int j = 0; ;j++) {
        if(j % BYTE == 0 && j != 0) {
            data[i] = byte_to_uchar((const unsigned char*)bits);
            if(data[i++] == '\0'){
               break; 
            }
        }
        pixel = image[y][x];
        if( shift % COMPONENTS == 0 ) {
            bits[j%BYTE] = (pixel->blue & 1) + CERO;
        }
        else if(shift % COMPONENTS == 1) {
            bits[j%BYTE] = (pixel->green & 1) + CERO;
        }
        else {
            bits[j%BYTE] = (pixel->red & 1)  + CERO;
        }
        shift+= hop;
        
        x = (shift/COMPONENTS)  % width, y = height-1-((shift/COMPONENTS) / width);
    }
    size = i;

    free(bits);
    free(data);

    return size;
}


// extracts a stream of bytes from an image
static unsigned char* extract(pixel*** image, int width, int height, int hop) {
    int shift =0;
    long data_size = extract_data_size(image, width, height, hop, &shift);
    printf("data_size = %ld\n", data_size);
    int extension_size = calculate_extension_size(image, width, height, data_size, hop, shift);
    long stream_size = SIZE_OF_LONG_IN_BYTES + data_size + extension_size;

    printf("extention_size = %d\n", extension_size);
    printf("data_size = %ld\n", data_size);
    printf("stream_size = %ld\n", stream_size);

    // array of bytes that will contain the stream
    unsigned char* stream = malloc(sizeof(*stream)*(stream_size));
    
    // array of bits that will be reused to extract bits and calculate each byte
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);

    // i is index for stream of bytes
    int i=0, x = 2, y = height-1;
    pixel* pixel;
    int restart_point = x;
     // we will extract every embeded bit in the image
    for(int j = 0, shift=0; j < stream_size*BYTE ; j++) {
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
        x = (shift/COMPONENTS) % width, y = height-1-((shift/COMPONENTS) / width);  
    }
    // update the stream array with its last byte
    stream[i++] = byte_to_uchar((const unsigned char*)bits);

    free(bits);    
    return stream;
}


int run_lsbi_embed(information* info, const unsigned char* stream, long stream_size) {
    printf("Starting embed\n");
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;
    pixel*** image = info->matrix;
    
    pixel* first_pixel = image[0][0];
    const int hop = first_pixel->blue;
    
    //process rc4
    const unsigned char* enc_stream = rc4(image, stream, true);
    // printf("%s \n", (char *)enc_stream);
    long stream_len = strlen(enc_stream);  
    
    embed(enc_stream, stream_len, image, width, height, hop);

    return SUCCESS;
}


unsigned char* run_lsbi_extract(information* info) {
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;
    pixel*** image = info->matrix;
    pixel* first_pixel = image[0][0];
    const int hop = first_pixel->blue;  

    //process rc4
    return rc4(image, extract(image, width, height, hop), false);
}