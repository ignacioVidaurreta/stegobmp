#include <stdio.h>

#include "include/bmp.h"

// TODO: chequear si validamos aca o en main que el archivo sea null
// o si lo hacemos en ambos lados
pixel** bmp_to_matrix(const char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    if(file == NULL) {
        return NULL;
    }
    unsigned char header[52];
    // 52 bytes are header bytes
    fscanf(file, "%s", header);
    printf("Header:\n\n");
    for(int i=0; i<52; i++) {
        printf("%c",header[i]);
    }
    pixel** matrix = NULL;
    // the rest is the image body (variable length)
    return matrix;
}
