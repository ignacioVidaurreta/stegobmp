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

#define MAX_EXTENSION_LEN 10
#define DWORD_SIZE 4
#define CERO '0'
#define ONE '1'


int bin_to_dec(const unsigned char* str, int n_bits) {
    int num = 0;
    for(int i = 0 ; i < n_bits ; i++) {
        num += (str[i]-'0')*pow(2,n_bits-i-1);
    }
    return num;
}

unsigned char byte_to_uchar(const unsigned char* bits) {
    int num = bin_to_dec(bits, 8);
    return (unsigned char)num;
}

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
    
    // for(int i=0; i<8; i++) {
    //     printf("%c",bits[i]);
    // }
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
    // for(int i=0; i<len; i++) {
    //     printf("%d\n", buffer[i]);
    // }
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

// TODO: eventually include program config for logs
char* get_extension(char* filename){
    char* raw_type = apply_file_cmd(filename);

    // TODO: memory leak
    char* ext = malloc(MAX_EXTENSION_LEN*sizeof(char));

    ext = translate_raw_to_ext(raw_type);

    free(raw_type);
    return ext;
}

// TODO: eventually include program config for logs
file_data* get_file_information(char* filename) {
    file_data* data = malloc(sizeof(*data));
    data->filename = filename;
    store_byte_repr_and_size(data);
    data->extension = get_extension(filename);
    return data;
}

void append_filelen_to_stream(unsigned char* stream, int filelen) {
    stream[0] = (filelen >> 24) & 0xFF;
    stream[1] = (filelen >> 16) & 0xFF;
    stream[2] = (filelen >> 8) & 0xFF;
    stream[3] = filelen & 0xFF;
    // printf("Filelen: %d -- Hexa: %x, %x, %x, %x\n",filelen, stream[0],stream[1],stream[2],stream[3]);
}

void append_file_content_to_stream(unsigned char* stream, file_data* data) {
    int filelen = data->filelen;
    char* content = data->file_content;
    for(int i=0; i<filelen; i++) {
        stream[DWORD_SIZE + i] = (unsigned char)content[i];
    }
}

void appeend_extension_to_stream(unsigned char* stream, file_data* data) {
    int ext_len = strlen(data->extension);
    int filelen = data->filelen;
    for(int i=0; i<ext_len; i++) {
        stream[DWORD_SIZE + filelen + i] = (unsigned char)(data->extension[i]);
    }
    stream[DWORD_SIZE + filelen + ext_len] = '\0';
}

unsigned char* concatenate(file_data* data) {

    // 4 because of dword len of file
    // filelen for file content + 1
    // strlen(extension) + 1 because it's ".ext\0"
    unsigned char* stream = malloc(DWORD_SIZE + data->filelen + strlen(data->extension) + 1);

    // filelen | file content | extension
    append_filelen_to_stream(stream, data->filelen);
    append_file_content_to_stream(stream, data);
    appeend_extension_to_stream(stream, data);
    
    // print stream
    // long len = DWORD_SIZE + data->filelen + strlen(data->extension) + 1;
    // for(int i=0; i<len; i++) {
    //     if(i == 4) {
    //         printf("\n");
    //     }
    //     if(i == 4+data->filelen) {
    //         printf("\n");
    //     }
    //     printf("%c-", stream[i]);
    // }
    // printf("\n");

    return stream;
}

int get_filelen_from_stream(unsigned char* stream) {
    int filelen = (int)stream[3] | ( (int)stream[2] << 8 ) | ( (int)stream[1] << 16 ) | ( (int)stream[0] << 24 );;
    printf("Filelen obtained: %d", filelen);
    return filelen;
}

char* get_file_content_from_stream(int filelen, unsigned char* stream) {
    char* content = malloc(sizeof(unsigned char)*filelen);
    for(int i=0; i<filelen; i++) {
        content[i] = (char)stream[DWORD_SIZE + i];
    }
    return content;
}

char* get_extension_from_stream(int filelen, unsigned char* stream) {
    char* extension = malloc(MAX_EXTENSION_LEN*sizeof(char));
    int i = 0;
    while(stream[filelen + i]!='\0') {
        extension[i] = (char)stream[DWORD_SIZE + filelen + i];
        i++;
    }
    extension[DWORD_SIZE + filelen + i] = '\0';
    return extension;
}

/* stream:   extracted when running a lsb algorithm
   filename: for creating the file
*/
file_data* split(unsigned char* stream) {
    file_data* data    = malloc(sizeof(*data));
    
    data->filelen      = get_filelen_from_stream(stream);
    data->file_content = get_file_content_from_stream(data->filelen, stream);
    data->extension    = get_extension_from_stream(data->filelen, stream);

    return data;
}

// output_file_name MUST NOT include extension
int generate_output_file(file_data* data, char* output_file_name) {

    int file_name_len = strlen(output_file_name);
    int len = file_name_len + MAX_EXTENSION_LEN;
    char* file = malloc(sizeof(char)*len);
    int i = 0;
    for(; i<file_name_len; i++) {
        file[i] = output_file_name[i];
    }
    int j = 0;
    while(data->extension[j] != '\0') {
        file[i++] = data->extension[j++]; 
    }
    file[i] = '\0';

    printf("File name:%s\n",file);

    FILE* output = fopen(file, "w");
    if(output == NULL) {
        return 1; // error
    }
    fwrite(data->file_content,data->filelen, 1,output);
    fclose(output);
    return 0; // success
}