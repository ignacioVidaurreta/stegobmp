#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/steganography.h"
#include "../include/bmp.h"
#include "../include/lsb1.h"
#include "../include/print.h"
#include "../include/base.h"
#include "../include/logging.h" 
#include "../include/errors.h"

#define BYTE 8
#define COMPONENTS 3
#define SIZE_OF_LONG_IN_BYTES 4
#define SIZE_OF_LONG_IN_BITS (SIZE_OF_LONG_IN_BYTES*BYTE)
#define BLOCK_FOR_EXTENSION_SIZE 15  // this is an estimate block to read extension
#define HOP 4
#define BYTES_RESERVED_FOR_KEY 6

// NOTE: stream_size != data_size
// stream = encryption_key || data_size || data || extension
// stream_size = BYTES_RESERVED_FOR_KEY + SIZE_OF_LONG_IN_BYTES + data_size + extension_size


// embeds a stream of bytes in image. If the size of stream is too large, 
// the function logs an error and returns
static int embed(const unsigned char* stream, int stream_size, pixel*** image, int width, int height) {

    // log error if stream_size it's too long to insert in image
    // The longest that it will be able to fit is the number of
    // elements in the matrix width*height*components is in bytes
    if((stream_size * BYTE) > (width * height * COMPONENTS)) {
        log_error_aux("Can't embed image: size exceeds available space");
        return ERROR_SIZE;
    }

    // array of bits that will be reused to calculate each byte to embed in bits
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);

    // i is index for stream of bytes
    int i = 0, x = 0, y = height-1;
    pixel* pixel;

    int restart_point = 0;
    // we will embed every bit of every byte in the image
    for(int j = 0, shift  = 0; j < stream_size*BYTE; j+=1) {
        
        // update the bits array with next byte
        if(j % BYTE == 0) {
            uchar_to_byte(bits, stream[i++]);
        }

       // Always grab the pixel (this could be improved in a future )
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
        shift += HOP;
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
static long extract_data_size(pixel*** image, int width, int height) {
    long size = 0;
    int x,y;
    long shift = BYTES_RESERVED_FOR_KEY*BYTE;
    x = (shift/COMPONENTS) % width;
    y = height-1-((shift/COMPONENTS) / width);
    pixel* pixel;

    // we need to get first 32 bits
    for(int j = shift; j < SIZE_OF_LONG_IN_BITS ; j++) {
        // get next pixel and update size, else only update size
        if( j % COMPONENTS == 0) {
            pixel = image[y][x];

            // moving through the matrix of pixels
            if(x+1 == width) { 
                y--;
                x=0;
            }
            else {
                x++;
            }
            size += (pixel->blue & 1) * pow(2,SIZE_OF_LONG_IN_BITS-j-1);
        }
        else if(j % COMPONENTS == 1) {
            size += (pixel->green & 1) * pow(2,SIZE_OF_LONG_IN_BITS-j-1);
        }
        else {
            size += (pixel->red & 1) * pow(2,SIZE_OF_LONG_IN_BITS-j-1);
        }
    }
    return size;
}


// calculates extension_size which resides after data_size and data in stream
// the index for the stream after data_size and data is shift = (SIZE_OF_LONG_IN_BYTES + data_size)*BYTE
// we read from shift (where '.' should be) until we find '\0'
// and we convert every 8 bits to a byte to check so
static int calculate_extension_size(pixel*** image, int width, int height, long data_size) {
    int size = 0, i =0;
    long shift = (BYTES_RESERVED_FOR_KEY + SIZE_OF_LONG_IN_BYTES + data_size)*BYTE;
    int x = (shift/COMPONENTS) % width, y = height-1-((shift/COMPONENTS) / width);

    unsigned char* data = malloc(sizeof(*data)*(BLOCK_FOR_EXTENSION_SIZE));
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);

    pixel* pixel;
    for(int j = shift; ; j++) {
        if(j % BYTE == 0 && j != 0) {
            data[i] = byte_to_uchar((const unsigned char*)bits);
            if(data[i] == '\0')
                break;
            i++;
        }
        
        if( j % COMPONENTS == 0) {
            pixel = image[y][x];
            if(x+1 == width) { 
                y--;
                x=0;
            }
            else {
                x++;
            }
            bits[j%BYTE] = (pixel->blue & 1) + CERO;
        }
        else if(j % COMPONENTS == 1) {
            bits[j%BYTE] = (pixel->green & 1)  + CERO;
        }
        else {
            bits[j%BYTE] = (pixel->red & 1)  + CERO;
        }
    }
    size = i;

    free(bits);
    free(data);

    return size;
}


// extracts a stream of bytes from an image
static unsigned char* extract(pixel*** image, int width, int height) {
    long data_size = extract_data_size(image, width, height);
    int extension_size = calculate_extension_size(image, width, height, data_size);
    long stream_size = BYTES_RESERVED_FOR_KEY +SIZE_OF_LONG_IN_BYTES + data_size + extension_size;

    // array of bytes that will contain the stream
    unsigned char* stream = malloc(sizeof(*stream)*(stream_size));
    
    // array of bits that will be reused to extract bits and calculate each byte
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);

    // i is index for stream of bytes
    int i=0, x = 0, y = height-1;
    pixel* pixel;
    int restart_point = 0;
     // we will extract every embeded bit in the image
    for(int j = 0, shift=0; j < stream_size*BYTE ; j++) {

        // update the stream array with last filled byte
        if(j % BYTE == 0 && j != 0) {
            stream[i++] = byte_to_uchar((const unsigned char*)bits);
        }

        pixel = image[y][x];
        
        if( j % COMPONENTS == 0) {
            bits[j%BYTE] = (pixel->blue & 1) + CERO;
        }
        else if(j % COMPONENTS == 1) {
            bits[j%BYTE] = (pixel->green & 1)  + CERO;
        }
        else {
            bits[j%BYTE] = (pixel->red & 1)  + CERO;
        }

        shift += HOP;
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
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;
    pixel*** image = info->matrix;
    
    pixel* first_pixel = image[0][0];
    const int hop = first_pixel->blue;
    
    printf("Hop: %d\n", hop);
    //process rc4
    const unsigned char* enc_stream = rc4(image, stream, true);
    //concatenate key in front

    //print_array(stream,stream_size);
    //printf("stream_size: %ld\n", stream_size);

    return ERROR_SIZE; //embed(stream, stream_size, image, width, height);
}


unsigned char* run_lsbi_extract(information* info) {
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;
    pixel*** image = info->matrix;
    
    return extract(image, width, height);
}