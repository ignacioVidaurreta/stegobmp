#ifndef BMP_H
#define BMP_H

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

pixel** bmp_to_matrix(const char* filename);






#endif /* BMP_H */