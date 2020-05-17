#include <stdio.h>
#include <stdlib.h>

#include "../include/bmp.h"

#define RGB_COLOR_SIZE 8
#define BYTE 8

void print_pixel(pixel* pixel) {
        printf(" %d %d %d |", pixel->blue, pixel->green, pixel->red);
}

void print_image_matrix(pixel*** image, int width, int height) {
    for(int y = 0 ; y < height ; y++) {
        for(int x = 0 ; x < width ; x++ ) {
            pixel* pixel = image[y][x];
            print_pixel(pixel);
        }
        printf("\n");
    }
}

void print_array(unsigned char* arr, int size) {
    for (int i = 0 ; i < size ; i++)
        printf("%c", arr[i]);
}

//TODO: error message if image can't embed the size of the data
void embed(unsigned char* data, int data_size, pixel*** image, int width, int height) {
    int i = 0; //index for data

    for(int y = height-1 ; y >= 0 ; y--) {
        for(int x = 0 ; x < width ; x++ ) {
            pixel* pixel = image[y][x];
            if(i < data_size){
                pixel->blue = pixel->blue & ~1 | (data[i++]-'0');
                if(i < data_size) {
                    pixel->green = pixel->green & ~1 | (data[i++]-'0');
                    if(i < data_size) {
                        pixel->red = pixel->red & ~1 | (data[i++]-'0');
                    }
                }
            }
        }
    }
}

unsigned char* extract(pixel*** image, int width, int height) {
    int data_size = 1*BYTE;//TODO: get data from first 4 bytes

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

int main(void) {
    //---------------image---------------

    int width = 2;
    int height = 2;

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
   
    image[0][0]->blue = 100;
    image[0][0]->green = 144;
    image[0][0]->red = 241;

    image[0][1]->blue = 100;
    image[0][1]->green = 144;
    image[0][1]->red = 241;

    image[1][0]->blue = 100;
    image[1][0]->green = 144;
    image[1][0]->red = 241;

    image[1][1]->blue = 100;
    image[1][1]->green = 144;
    image[1][1]->red = 241;   
    
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



