#ifndef BMP_H
#define BMP_H

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} pixel;

typedef struct {
    pixel*** matrix;
    unsigned int height;
    unsigned int width;
} information;

information* bmp_to_matrix(const char* filename);






#endif /* BMP_H */