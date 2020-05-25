#include <stdio.h>

#include "../include/bmp.h"
#include "../include/print.h"


void print_pixel(pixel* pixel) {
        printf(" %d %d %d |", pixel->blue, pixel->green, pixel->red);
}

void print_image_matrix(pixel*** image, int width, int height) {
    // Note: matrix is printed from top to bottom, from left to right
    // while the steganography acctually starts from bottom to top,
    // from left to right. So, for debuggin purposes change 'y = 0' 
    // for 'y = height-1'. This way, the last row of the matrix
    // (the first to be inserted with data) will be printed ;D
    pixel* pixel;
    printf("\nMatrix\n");
    for(int y = 0 ; y < height ; y++) {
        for(int x = 0 ; x < width ; x++ ) {
            pixel = image[y][x];
            print_pixel(pixel);
        }
        printf("\n");
    }
}

void print_array(unsigned char* arr, int size) {
    for (int i = 0 ; i < size ; i++)
        printf("%d ", arr[i]);
    printf("\n");    
}