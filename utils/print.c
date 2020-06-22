#include <stdio.h>

#include "../include/bmp.h"
#include "../include/print.h"

void print_array(const unsigned char* arr, int size) {
    for (int i = 0 ; i < size ; i++)
        printf("%d ", arr[i]);
    printf("\narray size = %d\n\n",size);    
}

void print_stream(const unsigned char* stream, long len, long filelen) {
    for(int i=0; i<len; i++) {
        if(i == 4) {
            printf("\n");
        }
        if(i == 4+filelen) {
            printf("\n");
        }
        printf("%c-", stream[i]);
    }
    printf("\n");
}

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

void print_image_header(bmp_header* header) {
    printf("Size of struct %ld\n\n", sizeof(*header));
    printf("Type: %d\n", header->type);
    printf("File size: %d\n", header->bmp_size);
    printf("Reserved 1: %d\n", header->reserved_1);
    printf("Reserved 2: %d\n", header->reserved_2);
    printf("Data offset: %d\n", header->bmp_data_offset);
    printf("Header size: %d\n", header->bmp_header_size);
    printf("Width: %d\n", header->bmp_width);
    printf("Height: %d\n", header->bmp_height);
    printf("Plane Number: %d\n", header->plane_number);
    printf("Point size: %d\n", header->point_size);
    printf("Compression: %d\n", header->compression);
    printf("Image size: %d\n", header->image_size);
    printf("Horizontal Resolution: %d\n", header->horizontal_resolution);
    printf("Vertical Resolution: %d\n", header->vertical_resolution);
    printf("Colour Table Size: %d\n", header->colour_table_size);
    printf("Important Colour Counter: %d\n", header->important_colour_counter);
}