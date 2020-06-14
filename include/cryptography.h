#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stdlib.h>

/* Returns a 160 bits compressed password. */
unsigned char* compress_password(const char* password);

/* Appends a 4 byte number at the beginning of a stream. */
void append_len_to_stream(unsigned char* stream, int len);

/* Returns a 4 byte number represented by the first 4 bytes at the beginning of a stream. */
long get_len_from_stream(unsigned char* stream);

/* Returns a n byte initialization vector */
unsigned char* get_iv(int bytes);

#endif /*   ENCRYPTION_H */