#ifndef BASE_H
#define BASE_H

#define TXT "ASCII text"
#define PDF "PDF" //EN ESTE CASO TIENE QUE HACER UN CONTAINS
#define JPEG "JPEG"
#define PNG "PNG"
#define C "C source, ASCII text"

#include "parser.h"

int bin_to_dec(const unsigned char* str, int n_bits);
char* get_byte_repr();
char* apply_file_cmd(char* filename);
char* translate_raw_to_ext(char* raw_type, struct config* config);
char* get_extension(char* filename, struct config* config);


#endif /*   BASE_H */