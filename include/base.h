#ifndef BASE_H
#define BASE_H

#define CERO '0'
#define ONE '1'

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


int get_most_signifcant_bit(unsigned char* bits, int len);

#endif /*   BASE_H */