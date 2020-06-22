#include <math.h>

#include "../include/base.h"

#define MAX_EXTENSION_LEN 10
#define DWORD_SIZE 4

// set of n bits to decimal
int bin_to_dec(const unsigned char* str, int n_bits) {
    int num = 0;
    for(int i = 0 ; i < n_bits ; i++) {
        num += (str[i]-CERO)*pow(2,n_bits-i-1);
    }
    return num;
}

// bits to byte
unsigned char byte_to_uchar(const unsigned char* bits) {
    int num = bin_to_dec(bits, 8);
    return (unsigned char)num;
}

// byte to bits
void uchar_to_byte(unsigned char* bits, unsigned char c) {
    int decimal_representation = (int)c;
    
    for(int i=7;i>=0;i--) {
        if(decimal_representation>0) {
            bits[i] = decimal_representation%2 == 0 ? CERO : ONE;    
            decimal_representation = decimal_representation/2;    
        }
        else {
            bits[i] = CERO;
        }
    }
}

int get_most_signifcant_bit(unsigned char* bits, int len) {
    for(int i = 0; i < len ;  i++) {
        if(bits[i] == ONE) {
            return pow(2, len-i-1);
        }
    }
    return 256;
}