#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bmp.h"
#include "../include/print.h"

#define RGB_COLOR_SIZE 8
#define BYTE 8
#define DATA_SIZE 2

static int embed_message(int i, pixel* pixel, int data_size, unsigned char* data ){
    if(i < data_size){
        pixel->blue = (pixel->blue & ~1) | (data[i++]-'0');
        if(i < data_size) {
            pixel->green = (pixel->green & ~1) | (data[i++]-'0');
            if(i < data_size) {
                pixel->red = (pixel->red & ~1) | (data[i++]-'0');
            }
        }
    }

    return i;
}

static void embed(unsigned char* data, int data_size, pixel*** image, int width, int height) {
    int i = 0; // Data index
    int hop = 2;
    int y = height-1;
    int x = 0;
    for (; y>=0; y--){
        for(; x < width;){
            pixel* pixel = image[y][x];
            i = embed_message(i, pixel, data_size, data);
            x+=hop;
        }
        x = x % width;
    }
}

static int extract_data(int i, pixel* pixel, int data_size, char* data){
    if(i < data_size){
        data[i++] = (pixel->blue & 1) + '0';
        if(i < data_size) {
            data[i++] = (pixel->green & 1)  + '0';
            if(i < data_size) {
                data[i++] = (pixel->red & 1)  + '0';
            }
        }
    }

    return i;
}
static unsigned char* extract(pixel*** image, int width, int height) {
    int data_size = DATA_SIZE *BYTE;//TODO: get data from first 4 bytes

    int i = 0;
    int hop = 4;
    unsigned char* data = malloc(sizeof(*data)*data_size);
    int y = height-1;
    int x = 0;
    for(; y >= 0 ; y--) {
        for(; x < width ;) {
            pixel* pixel = image[y][x];
            i = extract_data(i, pixel, data_size, data);
            x+=hop;
        }
        x = x % width;
    }
    return data;
}


void run_lsbi(information* info){
    //---------------image---------------

    int width = info->width;
    int height = info->height;

    pixel*** image = info->matrix;

    //---------------data---------------

    const unsigned char * data = "0110110101101101";
    int data_size = DATA_SIZE * BYTE;

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


    // free pixels
    for(int y = height-1 ; y >= 0 ; y--) {
        for(int x = 0 ; x < width ; x++ )
            free(image[y][x]);
    }

    // free columns
    for(int y = height-1 ; y >= 0 ; y--) {
        free(image[y]);
    }

    //free rows
    free(image);

  
}