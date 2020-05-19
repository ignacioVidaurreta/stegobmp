#include <stdio.h>
#include <math.h>
#include "../include/base.h"

int bin_to_dec(const unsigned char* str, int n_bits) {
    int num = 0;
    for(int i = 0 ; i < n_bits ; i++) {
        num += (str[i]-'0')*pow(2,n_bits-i-1);
    }
    return num;
}