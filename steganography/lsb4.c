#include <stdio.h>
#include <stdlib.h>

#include "../include/steganography.h"
#include "../include/bmp.h"
#include "../include/lsb1.h"
#include "../include/print.h"
#include "../include/base.h"
#include "../include/logging.h"
#include "../include/errors.h"

// NOTE: stream_size != data_size
// stream = data_size || data || extension
// stream_size = DWORD + data_size + extension_size


// embeds a stream of bytes in image. If the size of stream is too large, 
// the function logs an error and returns
static int embed(const unsigned char* stream, int stream_size, pixel*** image, int width, int height) {

    // log error if stream_size it's too long to insert in image
    //printf("\n %ld , %ld\n", stream_size*BYTE,width * height * COMPONENTS);
    if((stream_size * BYTE) > (width * height * COMPONENTS)) {
        log_error_aux("Can't embed image: size exceeds available space");
        return ERROR_SIZE;
    }

    // array of bits that will be reused to calculate each byte to embed in bits
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);

    // i is index for stream of bytes
    int i = 0, x = 0, y = 0;
    pixel* pixel;

    // we will embed every bit of every byte in the image
    for(int j = 0; j <= stream_size*BYTE; j+=HALF_BYTE) {
        
        // update the bits array with next byte
        if(j % BYTE == 0) {
            uchar_to_byte(bits, stream[i++]);
        }

        //printf("%d ", bin_to_dec(bits+(j%BYTE),HALF_BYTE));

        // as it is lsb4 we will embed 4 bits in each component of 
        // the 3 components of every pixel. that is why we increment 
        // by 4 bits and also why the possible modules of COMPONENTS*HALF_BYTE (12)
        // are 0, 4 or 8
        //
        // get next pixel and embed 4 bits, else only embed 4 bits in corresponding component
        if(j % (COMPONENTS*HALF_BYTE) == 0) {
            pixel = image[y][x];

            // moving through the matrix of pixels
            if(x+1 == width) {
                y++;
                x=0;
            }
            else {
                x++;
            }
            pixel->blue = (pixel->blue & ~15) | bin_to_dec(bits+(j%BYTE),HALF_BYTE);
        }
        else if(j % (COMPONENTS*HALF_BYTE) == 4) {
            pixel->green = (pixel->green & ~15) | bin_to_dec(bits+(j%BYTE),HALF_BYTE);
        }
        else {
            pixel->red = (pixel->red & ~15) | bin_to_dec(bits+(j%BYTE),HALF_BYTE);
        }
    }
    free(bits);
    return SUCCESS;
}


// extracts last 4 bits from a pixel component and saves it in the bits array
static void extract_from_component(unsigned char num, unsigned char * bits, int bits_index) {
    int index = bits_index;
    for(int i = HALF_BYTE ; i > 0 ; i--) {
        bits[index++] = ((num & (1 << (i-1))) != 0) + CERO;
    }
}


// extracts data_size which resides in first 4 bytes of embeded stream (size of long)
static long extract_data_size(pixel*** image, int width, int height) {
    long size = 0;
    int x = 0, y = 0;
    pixel* pixel;
    
    unsigned char* first_32_bits = malloc(sizeof(unsigned char)*SIZE_OF_LONG_IN_BITS);

    // we need to get first 32 bits
    for(int j = 0; j < SIZE_OF_LONG_IN_BITS ; j+=HALF_BYTE) {
        // get next pixel and update size, else only update size
        if(j % (COMPONENTS*HALF_BYTE) == 0) {
            pixel = image[y][x];

            // moving through the matrix of pixels
            if(x+1 == width) { 
                y++;
                x=0;
            }
            else {
                x++;
            }
            extract_from_component(pixel->blue, first_32_bits, j);
        }
        else if(j % (COMPONENTS*HALF_BYTE) == 4) {
            extract_from_component(pixel->green, first_32_bits, j);
        }
        else {
            extract_from_component(pixel->red, first_32_bits, j);
        }
    }

    size = bin_to_dec(first_32_bits,SIZE_OF_LONG_IN_BITS);

    free(first_32_bits);

    return size;
}


// calculates extension_size which resides after data_size and data in stream
// the index for the stream after data_size and data is shift = (DWORD + data_size)*BYTE
// we read from shift (where '.' should be) until we find '\0'
// and we convert every 8 bits to a byte to check so
static int calculate_extension_size(pixel*** image, int width, int height, long data_size) {
    int size = 0, i =0;
    long shift = (DWORD + data_size)*BYTE;
    int x = (shift/(COMPONENTS*HALF_BYTE)) % width, y = ((shift/(COMPONENTS*HALF_BYTE)) / width);

    unsigned char* data = malloc(sizeof(*data)*(BLOCK_FOR_EXTENSION_SIZE));
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);

    pixel* pixel;
    for(int j = shift; ; j+=HALF_BYTE) {
        if(j % BYTE == 0 && j != shift) {
            data[i] = byte_to_uchar((const unsigned char*)bits);
            if(data[i++] == '\0')
                break;
        }
        
        if( j % (COMPONENTS*HALF_BYTE) == 0 || j == shift ) {
            pixel = image[y][x];
            if(x+1 == width) { 
                y++;
                x=0;
            }
            else {
                x++;
            }
        }

        if(j % (COMPONENTS*HALF_BYTE) == 0) {
            extract_from_component(pixel->blue, bits, j%BYTE);
        }
        else if(j % (COMPONENTS*HALF_BYTE) == 4) {
            extract_from_component(pixel->green, bits, j%BYTE);
        }
        else {
            extract_from_component(pixel->red, bits, j%BYTE);
        }
    }
    size = i;

    free(bits);
    free(data);

    return size;
}


// extracts a stream of bytes from an image
static unsigned char* extract(pixel*** image, int width, int height, int is_encrypted) {
    long data_size = extract_data_size(image, width, height);
    int extension_size = 0;
    if(!is_encrypted)
        extension_size = calculate_extension_size(image, width, height, data_size);
    long stream_size = DWORD + data_size + extension_size;

    // printf("extention_size = %d\n", extension_size);
    // printf("data_size = %ld\n", data_size);
    // printf("stream_size = %ld\n", stream_size);
    
    // array of bytes that will contain the stream
    unsigned char* stream = malloc(sizeof(*stream)*(stream_size));
    
    // array of bits that will be reused to extract bits and calculate each byte
    unsigned char* bits = malloc(sizeof(unsigned char)*BYTE);

    // i is index for stream of bytes
    int i=0, x = 0, y = 0;
    pixel* pixel;

    // we will extract every embeded bit in the image
    for(int j = 0; j <= stream_size*BYTE ; j+=HALF_BYTE) {

        // update the stream array with last filled byte
        if(j % BYTE == 0 && j != 0) {
            stream[i++] = byte_to_uchar((const unsigned char*)bits);
        }

        // as it is lsb4 there are 4 bits embeded in each component of 
        // the 3 components of every pixel. that is why we increment 
        // by 4 bits and also why the possible modules of COMPONENTS*HALF_BYTE (12)
        // are 0, 4 or 8
        //
        // get next pixel and extract 4 bits, else only extract 4 bits
        if(j % (COMPONENTS*HALF_BYTE) == 0) {
            pixel = image[y][x];

            // moving through the matrix of pixels
            if(x+1 == width) {
                y++;
                x=0;
            }
            else {
                x++;
            }
            extract_from_component(pixel->blue, bits, j%BYTE);
        }
        else if(j % (COMPONENTS*HALF_BYTE) == 4) {
            extract_from_component(pixel->green, bits, j%BYTE);
        }
        else {
            extract_from_component(pixel->red, bits, j%BYTE);
        }
    }
    // update the stream array with its last byte
    stream[i++] = byte_to_uchar((const unsigned char*)bits);

    //printf("\n");
    //print_array(stream,stream_size);

    free(bits);            
    return stream;
}


int run_lsb4_embed(information* info, const unsigned char* stream, long stream_size) {
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;
    pixel*** image = info->matrix;

    //print_array(stream,stream_size);
    //printf("stream_size: %ld\n", stream_size);

    return embed(stream, stream_size, image, width, height);
}


unsigned char* run_lsb4_extract(information* info, int is_encrypted) {
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;
    pixel*** image = info->matrix;
    
    return extract(image, width, height, is_encrypted);
}
