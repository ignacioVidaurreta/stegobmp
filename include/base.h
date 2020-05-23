#ifndef BASE_H
#define BASE_H

#define TXT "ASCII text"
#define PDF "PDF" //EN ESTE CASO TIENE QUE HACER UN CONTAINS
#define JPEG "JPEG"
#define PNG "PNG"
#define C "C source, ASCII text"

#include "parser.h"

typedef struct {
    char*          filename;
    char*          extension;
    char*          file_content;
    int            filelen;
} file_data;

int bin_to_dec(const unsigned char* str, int n_bits);
char* get_byte_repr();
char* apply_file_cmd(char* filename);
char* translate_raw_to_ext(char* raw_type, struct config* config);
char* get_extension(char* filename, struct config* config);

// TODO: fix this method signature
file_data* get_file_information(char* filename);

/* Returns a stream with file content, extension and size concatenated
*/
unsigned char* concatenate(file_data* data);

/* Returns file content, extension, and size separated,
    obtained from a stream of bytes
*/
file_data* split(unsigned char* stream);



#endif /*   BASE_H */