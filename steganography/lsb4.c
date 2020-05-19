#include <stdio.h>
#include <stdlib.h>

#include "../include/bmp.h"
#include "../include/lsb1.h"
#include "../include/print.h"
#include "../include/base.h"

#define RGB_COLOR_SIZE 8
#define BYTE 8
#define HALF_BYTE 4

#define DATA_SIZE 2 // This is a magic number according to data so i don't forget to change it in two places :)
                    // This will change once we have a structure where we recieve data, its size and extention

//TODO: error message if image can't embed the size of the data
static void embed(const unsigned char* data, int data_size, pixel*** image, int width, int height) {
    int i = 0; //index for data

    for(int y = height-1 ; y >= 0 ; y--) {
        for(int x = 0 ; x < width ; x++ ) {
            pixel* pixel = image[y][x];
            if(i < data_size){
                pixel->blue = (pixel->blue & ~15) | bin_to_dec(data+i,HALF_BYTE);
                i += HALF_BYTE;
                if(i < data_size) {
                    pixel->green = (pixel->green & ~15) | bin_to_dec(data+i,HALF_BYTE);
                    i += HALF_BYTE;
                    if(i < data_size) {
                        pixel->red = (pixel->red & ~15) | bin_to_dec(data+i,HALF_BYTE);
                        i += HALF_BYTE;
                    }
                }
            }
        }
    }
}

static void extract_from_component(unsigned char num, unsigned char * data, int data_index) {
    int index = data_index;
    for(int i = HALF_BYTE ; i > 0 ; i--) {
        data[index++] = ((num & (1 << (i-1))) != 0) + '0';
    }
}

static unsigned char* extract(pixel*** image, int width, int height) {
    int data_size = DATA_SIZE *BYTE;//TODO: get data from first 4 bytes

    int i = 0;
    unsigned char* data = malloc(sizeof(*data)*data_size);

    for(int y = height-1 ; y >= 0 ; y--) {
        for(int x = 0 ; x < width ; x++ ) {
            pixel* pixel = image[y][x];
            if(i < data_size){
                extract_from_component(pixel->blue, data, i);
                i += HALF_BYTE;
                if(i < data_size) {
                    extract_from_component(pixel->green, data, i);
                    i += HALF_BYTE;
                    if(i < data_size) {
                        extract_from_component(pixel->red, data, i);
                        i += HALF_BYTE;
                    }
                }
            }
        }
    }
    return data;
}

void run_lsb4(information* info) {
    int width = info->width;
    int height = info->height;

    pixel*** image = info->matrix;
    
    //---------------data---------------

    const unsigned char * data = "0110110101101101";
    int data_size = DATA_SIZE * BYTE; // See DATA_SIZE define comment

    //---------------steganography---------------
    
    printf("Antes:\n");
    print_image_matrix(image, width, height);
    printf("\n");

    embed(data, data_size, image, width, height);

    printf("Despues:\n");
    print_image_matrix(image, width, height);
    printf("\n");

    printf("Extracted data: ");
    print_array(extract(image, width, height),data_size);
    printf("\n");
}

