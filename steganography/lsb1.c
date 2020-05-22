#include <stdio.h>
#include <stdlib.h>

#include "../include/bmp.h"
#include "../include/lsb1.h"
#include "../include/print.h"

#define RGB_COLOR_SIZE 8
#define BYTE 8

#define DATA_SIZE 2 // This is a magic number according to data so i don't forget to change it in two places :)
                    // This will change once we have a structure where we recieve data, its size and extention

//TODO: error message if image can't embed the size of the data
static void embed(const unsigned char* data, int data_size, pixel*** image, int width, int height) {
    int i = 0; //index for data

    for(int y = height-1 ; y >= 0 ; y--) {
        for(int x = 0 ; x < width ; x++ ) {
            pixel* pixel = image[y][x];
            if(i < data_size){
                pixel->blue = (pixel->blue & ~1) | (data[i++]-'0');
                if(i < data_size) {
                    pixel->green = (pixel->green & ~1) | (data[i++]-'0');
                    if(i < data_size) {
                        pixel->red = (pixel->red & ~1) | (data[i++]-'0');
                    }
                }
            }
        }
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
                data[i++] = (pixel->blue & 1) + '0';
                if(i < data_size) {
                    data[i++] = (pixel->green & 1)  + '0';
                    if(i < data_size) {
                        data[i++] = (pixel->red & 1)  + '0';
                    }
                }
            }
        }
    }
    return data;
}

void run_lsb1(information* info) {
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;

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