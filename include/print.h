#ifndef PRINT_H
#define PRINT_H

#include "bmp.h"

void print_pixel(pixel* pixel);
void print_image_matrix(pixel*** image, int width, int height);
void print_array(unsigned char* arr, int size);

#endif /* PRINT_H */