#ifndef FILE_H
#define FILE_H

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

/* Returns a file_data structure with everything needed to concatenate
   information to run through lbs1, lsb4 or lsbi */
file_data* get_file_information(char* filename);

/* Recovers, from stream, extension, file_content and filelen. 
   DOES NOT recover filename */
file_data* split(unsigned char* stream);

/* Prepares file_data into a single stream of bytes so it's ready
   to run thorugh lsb1, lsb4 or lsbi */
unsigned char* concatenate(file_data* data);

/* Once file_data is recovered from stream of bytes, generates an output
   file with file_content and extension, with name passed as an argument.
   output_file_name MUST NOT include extension */
int generate_output_file(file_data* data, char* output_file_name);

#endif /*   FILE_H */