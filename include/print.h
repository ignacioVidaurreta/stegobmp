#ifndef PRINT_H
#define PRINT_H

#include "bmp.h"

void print_pixel(pixel* pixel);
void print_image_matrix(pixel*** image, int width, int height);
void print_array(const unsigned char* arr, int size);
void print_stream(const unsigned char* stream, long len, long filelen);
void print_image_header(bmp_header* header);

#endif /* PRINT_H */