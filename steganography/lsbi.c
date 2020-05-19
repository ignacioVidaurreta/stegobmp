#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bmp.h"
#include "../include/print.h"

#define RGB_COLOR_SIZE 8
#define BYTE 8

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


static pixel*** create_image(int width, int height){
    //memory allocation for rows
    pixel*** image = malloc(sizeof(*image)*height);

    // memory allocation for columns
    for(int y = height-1 ; y >= 0 ; y--) {
        image[y] = malloc(sizeof(**image)*width);
    }

    // memory allocation for pixels
    for(int y = height-1 ; y >= 0 ; y--) {
        for(int x = 0 ; x < width ; x++ )
            image[y][x] = malloc(sizeof(pixel));
    }

    return image;
}

void initialize_image(pixel*** image){
    image[0][0]->blue = 100;
    image[0][0]->green = 144;
    image[0][0]->red = 241;

    image[0][1]->blue = 100;
    image[0][1]->green = 144;
    image[0][1]->red = 241;

    image[0][2]->blue = 100;
    image[0][2]->green = 144;
    image[0][2]->red = 241;

    image[0][3]->blue = 100;
    image[0][3]->green = 144;
    image[0][3]->red = 241;

    image[1][0]->blue = 100;
    image[1][0]->green = 144;
    image[1][0]->red = 241;

    image[1][1]->blue = 100;
    image[1][1]->green = 144;
    image[1][1]->red = 241;

    image[1][2]->blue = 100;
    image[1][2]->green = 144;
    image[1][2]->red = 241;

    image[1][3]->blue = 100;
    image[1][3]->green = 144;
    image[1][3]->red = 241;

    image[2][0]->blue = 100;
    image[2][0]->green = 144;
    image[2][0]->red = 241;

    image[2][1]->blue = 100;
    image[2][1]->green = 144;
    image[2][1]->red = 241;

    image[2][2]->blue = 100;
    image[2][2]->green = 144;
    image[2][2]->red = 241;

    image[2][3]->blue = 100;
    image[2][3]->green = 144;
    image[2][3]->red = 241;

    image[3][0]->blue = 100;
    image[3][0]->green = 144;
    image[3][0]->red = 241;

    image[3][1]->blue = 100;
    image[3][1]->green = 144;
    image[3][1]->red = 241;

    image[3][2]->blue = 100;
    image[3][2]->green = 144;
    image[3][2]->red = 241;

    image[3][3]->blue = 100;
    image[3][3]->green = 144;
    image[3][3]->red = 241;

}

void run_lsbi(){
    //---------------image---------------

    int width = 4;
    int height = 4;

    pixel*** image = create_image(width, height);


    initialize_image(image);

    //---------------data---------------

    unsigned char * data = "01101101";
    int data_size = 1 * BYTE;

    //---------------steganography---------------
    
    printf("Antes:\n");
    print_image_matrix(image, width, height);
    printf("\n");

    embed(data, data_size, image, width, height);

    printf("Despues:\n");
    print_image_matrix(image, width, height);
    printf("\n");

    // printf("Extracted data: ");
    // print_array(extract(image, width, height),data_size);
    // printf("\n");


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