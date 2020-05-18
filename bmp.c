#include <stdio.h>
#include <stdlib.h>

#include "include/bmp.h"

#define HEADER_SIZE 54

// unsigned short --> size 2
// unsigned int   --> size 4
typedef struct __attribute__((__packed__)) {
    unsigned short type;
    unsigned int bmp_size;
    unsigned short reserved_1;
    unsigned short reserved_2;
    unsigned int bmp_data_offset;
    unsigned int bmp_header_size;
    unsigned int bmp_width;
    unsigned int bmp_height;
    unsigned short plane_number;
    unsigned short point_size;
    unsigned int compression;
    unsigned int image_size;
    unsigned int horizontal_resolution;
    unsigned int vertical_resolution;
    unsigned int colour_table_size;
    unsigned int important_colour_counter;
}bmp_header;


void print_header_info(bmp_header* header) {
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

void print_pixel_matrix(pixel*** matrix, unsigned int height, unsigned int width) {
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            printf("[b:%d, g:%d, r:%d],", matrix[i][j]->blue, matrix[i][j]->green, matrix[i][j]->red);
        }
        printf("\n");
    }
}

void free_header(bmp_header* header) {
    free(header);
}

void free_pixel_matrix(pixel*** matrix, unsigned int height, unsigned int width) {
    for(int i=0; i<height; i++) {
        for(int j=0; j<width;j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
}

// TODO: chequear si validamos aca o en main que el archivo sea null
// o si lo hacemos en ambos lados
information* bmp_to_matrix(const char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    if(file == NULL) {
        perror("Error opening file\n");
        exit(1);
    }

    bmp_header* header = malloc(sizeof(*header));
    fread(header,sizeof(*header),1,file);

    unsigned int height = header->bmp_height;
    unsigned int width = header->bmp_width;
    pixel*** matrix = malloc(sizeof(*matrix)*height);
    for(int i=0; i<height; i++) {
        matrix[i] = malloc(sizeof(**matrix)*width);
        for(int j=0; j<width; j++) {
            matrix[i][j] = malloc(sizeof(pixel));
        }
    }
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            fread(matrix[i][j],sizeof(pixel),1,file);
        }
    }
    information* info = malloc(sizeof(*info));
    info->matrix = matrix;
    info->height = height;
    info->width = width;

    free_header(header);

    return info;
}
