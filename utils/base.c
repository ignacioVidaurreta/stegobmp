#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

#include "../include/base.h"
#include "../include/logging.h"

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

char* apply_file_cmd(char* filename){
   
    char *argv[3];
    argv[0] = filename;
    argv[1] = filename; // idk why it needs to be here twice tbh
    argv[2] = NULL;
    int pid = fork();

    if(pid == 0){
        execvp("scripts/filetype_to_file.sh", argv);
        perror("execv");
        exit(0);
    }else{
        int ret;
        waitpid(pid, &ret, 0); // Wait for child to finish
        return read_file_type("filetype.txt");
    }
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

    // int len = strlen(buffer); TODO INVESTIGATE WHY THIS ASSERT FAILS
    // printf("%ld, %d\n", filelen, len);
    // assert(filelen-1 == len);
    return buffer;
}

char* translate_raw_to_ext(char* raw_type, struct config* config){
    char* ret;
    bool err = false;
    if(strcmp(raw_type, TXT) == 0){
        ret = ".txt";
    }else if(strcmp(raw_type, C)==0){
        ret = ".c";
    }else{
        raw_type = strtok(raw_type, " "); // Get the first word
        if(strcmp(raw_type, PDF) == 0){
            ret = ".pdf";
        }else if(strcmp(raw_type, JPEG) == 0){
            ret = ".jpeg";
        }else if(strcmp(raw_type, PNG) == 0){
            ret = ".png";
        }else{
            err = true;
            ret = "ERR";
        }
    }

    log_extension(ret, err, config);
    return ret;

}

char* get_extension(char* filename, struct config* config){
    char* raw_type = apply_file_cmd(filename);
    
    char* ext = malloc(10*sizeof(char));

    ext = translate_raw_to_ext(raw_type, config);

    free(raw_type);
    return ext;

}