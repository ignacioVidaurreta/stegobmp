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
    fclose(fd);

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

int store_byte_repr_and_size(file_data* data){

    FILE*          fileptr;
    char*          buffer;
    long           filelen;

    char* filename = data->filename;

    fileptr = fopen(filename, "rb");                        // Open the file in binary mode
    if(fileptr == NULL) {                                   // Some kind of problem when opening the file
        return 1;
    }
    fseek(fileptr, 0, SEEK_END);                            // Jump to the end of the file
    filelen = ftell(fileptr);                               // Get the current byte offset in the file
    rewind(fileptr);                                        // Jump back to the beginning of the file

    data->filelen = filelen;                                // Stores file len on data structure  
    
    buffer = (char *)malloc(filelen * sizeof(char));        // Enough memory for the file
    fread(buffer, filelen, 1, fileptr);                     // Read in the entire file
    fclose(fileptr);                                        // Close the file

    data->file_content = buffer;                            // Stores file content on data structure  

    int len = strlen(buffer);
    // printf("%ld, %d\n", filelen, len);
    assert(filelen == len);

    return 0;
}

char* translate_raw_to_ext(char* raw_type){
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

    // log_extension(ret, err, config);
    return ret;

}

// TODO: config as argument
char* get_extension(char* filename){
    char* raw_type = apply_file_cmd(filename);

    // TODO: memory leak
    char* ext = malloc(10*sizeof(char));

    ext = translate_raw_to_ext(raw_type);

    free(raw_type);
    return ext;

}

// TODO: figure out WHY the len of the file wether I pass or not arguemtns
// specially since when I use arguments, wierd characters appear when reading the file

file_data* get_file_information(char* filename) {
    file_data* data = malloc(sizeof(*data));
    data->filename = filename;
    store_byte_repr_and_size(data);
    data->extension = get_extension(filename);
    return data;
}

unsigned char* concatenate(file_data* data) {
    return NULL;
}

file_data* split(unsigned char* stream) {
    return NULL;
}