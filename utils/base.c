#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../include/base.h"

int bin_to_dec(const unsigned char* str, int n_bits) {
    int num = 0;
    for(int i = 0 ; i < n_bits ; i++) {
        num += (str[i]-'0')*pow(2,n_bits-i-1);
    }
    return num;
}

char* get_byte_repr(char* filename){

    FILE *fileptr;
    char *buffer;
    long filelen;

    fileptr = fopen(filename, "rb");                     // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);                            // Jump to the end of the file
    filelen = ftell(fileptr);                               // Get the current byte offset in the file
    rewind(fileptr);                                        // Jump back to the beginning of the file

    buffer = (char *)malloc(filelen * sizeof(char) +1);     // Enough memory for the file
    fread(buffer, filelen, 1, fileptr);                     // Read in the entire file
    fclose(fileptr);                                        // Close the file

    buffer[filelen * sizeof(char) +1] = "\0";               // Insert the \n because we are not animals  

    printf("%ld  %d\n", filelen, strlen(buffer));

    return buffer;
}