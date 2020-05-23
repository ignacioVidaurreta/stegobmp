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

/* bits MUST BE an array of 8 unsigned chars. The function stores
   the unsigned char c as an array of 0s and 1s (bit representation)
*/
void uchar_to_byte(unsigned char* bits, unsigned char c);

/* bits MUST BE an array of 8 unsigned chars. The function returns
   the unsigned char corresponding to those bits.
*/
unsigned char byte_to_uchar(const unsigned char* bits);

/* returns the number corresponding to bit representation stored in
   str (qty of bits is n_bits)
*/
int bin_to_dec(const unsigned char* str, int n_bits);

#endif /*   BASE_H */