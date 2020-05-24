#include <stdio.h>
#include <stdlib.h>

#include "../include/bmp.h"
#include "../include/lsb1.h"
#include "../include/print.h"
#include "../include/base.h"

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

// static void embed_byte(unsigned char* byte, int curr_x, int curr_y, unsigned char* first_pixel_components_to_be_embeded) {
//     int i = 0;
//     unsigned char first_pixel_components_to_fill = *first_pixel_components_to_be_embeded;

//     for(int y = curr_y ; i < BYTE && y >= 0 ; y--) {
//         int x = 0;
//         if (y == curr_y) {
//             x = curr_x + 1; // first pixel to be embeded was already embeded
//         } 
//         for( x ; i < BYTE && x < width ; x++ ) {
//             pixel* pixel = image[y][x];
//             //if(i < data_size){
//                 pixel->blue = (pixel->blue & ~1) | (data[i++]-'0');
//                 if(i < BYTE) {
//                     pixel->green = (pixel->green & ~1) | (data[i++]-'0');
//                     if(i < BYTE) {
//                         pixel->red = (pixel->red & ~1) | (data[i++]-'0');
//                     }
//                     else
//                     {
//                         *first_pixel_components_to_be_embeded = 1;
//                     }
//                 }
//                 else
//                 {
//                     *first_pixel_components_to_be_embeded = 2;
//                 }
//             //}
//         }
//     }
// }

static unsigned char* extract(pixel*** image, int width, int height, long data_size) {
   // int data_size = ;//TODO: get data from first 4 bytes

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

void run_lsb1(information* info, unsigned char* data_stream) {
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;

    pixel*** image = info->matrix;
    
    //---------------data---------------

    const unsigned char * data = "0110110101101101";
    int data_size = DATA_SIZE * BYTE; // See DATA_SIZE define comment

    //int data_stream_size = get_filelen_from_stream(data_stream); // size in BYTES



    //---------------steganography---------------
    
    printf("Antes:\n");
    print_image_matrix(image, width, height);
    printf("\n");

    embed(data, data_size, image, width, height);

    printf("Despues:\n");
    print_image_matrix(image, width, height);
    printf("\n");

    printf("Extracted data: ");
    //print_array(extract(image, width, height),data_size);
    printf("\n");

}

void run_lsb1_embed(information* info, const unsigned char* data, long data_size) {
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;

    pixel*** image = info->matrix;

    printf("Data to embed: %s\n", (char*)data);
    printf("Data to embed size: %ld\n", data_size);


    embed(data, data_size, image, width, height);
}

void run_lsb1_extract(information* info, unsigned char* data, long data_size) {
    int width = info->header->bmp_width;
    int height = info->header->bmp_height;

    pixel*** image = info->matrix;

    unsigned char* data_in_bits = extract(image, width, height, data_size*8);

    print_array(data_in_bits,data_size);
    printf("extraccion en bits realizada \n");

    printf("Extracted data: %s", (char*)data_in_bits);

    for(int i = 0 ; i < data_size ; i++) {
        data[i] = byte_to_uchar(data_in_bits);
        data_in_bits+=8;
    }

}