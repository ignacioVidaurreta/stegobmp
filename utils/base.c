#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/base.h"

int bin_to_dec(const unsigned char* str, int n_bits) {
    int num = 0;
    for(int i = 0 ; i < n_bits ; i++) {
        num += (str[i]-'0')*pow(2,n_bits-i-1);
    }
    return num;
}

// TODO: Stop wasting so much memmory
char* read_file_type(char* file){
    FILE *fd;
    char *buffer = malloc(100*sizeof(char));

    fd = fopen(file, "r");
    char* filetype = malloc(100*sizeof(char));
    // https://stackoverflow.com/questions/20108334/traverse-file-line-by-line-using-fscanf/20108623
    fscanf(fd, "%s %30[^\n]\n", buffer, filetype);

    return filetype;
}

void get_stat(char* filename){
   
    char *argv[3];// = {"dummy.txt", NULL};
    argv[0] = "logging.c";
    argv[1] = "logging.c"; // idk why it needs to be here twice tbh
    argv[2] = NULL;
    int pid = fork();

    if(pid == 0){
        execvp("scripts/filetype_to_file.sh", argv);
        perror("execv");
        exit(0);
    }else{
        int ret;
        waitpid(pid, &ret, 0); // Wait for child to finish
        printf("%s\n", read_file_type("filetype.txt"));

    }
}

char* get_byte_repr(char* filename){

    FILE *fileptr;
    char *buffer;
    long filelen;
    get_stat(filename);
    fileptr = fopen(filename, "rb");                     // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);                            // Jump to the end of the file
    filelen = ftell(fileptr);                               // Get the current byte offset in the file
    rewind(fileptr);                                        // Jump back to the beginning of the file

    buffer = (char *)malloc(filelen * sizeof(char) +1);     // Enough memory for the file
    fread(buffer, filelen, 1, fileptr);                     // Read in the entire file
    fclose(fileptr);                                        // Close the file

    buffer[filelen * sizeof(char) +1] = "\0";               // Insert the \n because we are not animals  

   // printf("%ld  %d\n", filelen, strlen(buffer));

    return buffer;
}