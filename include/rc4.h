#ifndef RC4_NEW_H
#define RC4_NEW_H

#include "bmp.h"

    void swap(unsigned char *a, unsigned char *b);
    const unsigned char* get_key_from_image(pixel*** image, unsigned char* key);
    int create_encoding_stream(pixel*** image, unsigned char *encoding_stream);
    int apply(unsigned char *encoding_stream, char *stream, int len, unsigned char* response);
    unsigned char* RC4(pixel*** image, unsigned char* stream, long len);


#endif /*RC4_NEW_H */