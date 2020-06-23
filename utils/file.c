#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <sys/stat.h>

#include "../include/base.h"
#include "../include/errors.h"
#include "../include/file.h"
#include "../include/logging.h"

#define MAX_EXTENSION_LEN 10
#define DWORD_SIZE 4

char* read_file_type(char* file){
    FILE *fd;
    
    char *buffer = malloc(100*sizeof(char));
    if(buffer == NULL){
        log_error_aux("Allocation of memory failed on read_file_type");
        return NULL;
    }
        
    
    fd = fopen(file, "r");
    if(fd == NULL) {
        log_error_aux("File opening failed ono read_file_type");
        free(buffer);
        return NULL;
    }
    
    char* filetype = malloc(100*sizeof(char));
    if(filetype == NULL) {
        log_error_aux("Allocation of memory failed on read_file_type");
        free(buffer);
        fclose(fd);
        return NULL;
    }
    
    // https://stackoverflow.com/questions/20108334/traverse-file-line-by-line-using-fscanf/20108623
    fscanf(fd, "%s %30[^\n]\n", buffer, filetype);

    free(buffer);
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
        log_error_aux("Execv failed");
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
    if(fileptr == NULL) {
        log_error_aux("File opnening failed on store_byte_repr_and_size");
        return FAILURE;
    }
    
    int fd = fileno(fileptr);
    struct stat buff;
    fstat(fd, &buff);
    // MAX 2GB
    filelen = buff.st_size;
    data->filelen = filelen;  
    
    buffer = (char *)malloc(filelen * sizeof(char));
    if(buffer == NULL) {
        log_error_aux("Allocation of memory failed on store_byte_repr_and_size");
        fclose(fileptr);
        return FAILURE;
    }

    fread(buffer, 1, filelen, fileptr);
    buffer[filelen*sizeof(char)] = '\0';
    
    fclose(fileptr);

    data->file_content = buffer;

    return SUCCESS;
}

char* translate_raw_to_ext(char* ret, char* raw_type){
    // bool err = false;
    if(strcmp(raw_type, TXT) == 0){
        strcpy(ret, ".txt");
    }else if(strcmp(raw_type, C)==0){
        strcpy(ret, ".c");
    
    }else{
        raw_type = strtok(raw_type, " "); // Get the first word
        if(strcmp(raw_type, PDF) == 0){
            strcpy(ret, ".pdf");
        }else if(strcmp(raw_type, JPEG) == 0){
            strcpy(ret, ".jpeg");
        }else if(strcmp(raw_type, PNG) == 0){
            strcpy(ret, ".png");
        }else{
            // err = true;
            strcpy(ret, "ERR");
        }
    }

    return ret;
    // log_extension(ret, err, config);

}

char* get_extension(char* filename){
    char* raw_type = apply_file_cmd(filename);

    char* ext = malloc(MAX_EXTENSION_LEN*sizeof(char));
    if(ext == NULL) {
        log_error_aux("Allocation of memory failed on get_extension");
        return NULL;
    }

    ext = translate_raw_to_ext(ext, raw_type);

    free(raw_type);
    return ext;
}

file_data* get_file_information(char* filename) {
    
    file_data* data = malloc(sizeof(*data));
    if(data == NULL){    
        log_error_aux("Allocation of memory failed on get_file_information");
        return NULL;
    } 

    data->filename = filename;
    
    int result = store_byte_repr_and_size(data);
    if(result != SUCCESS) {
        log_error_aux("Could not get file and filelen on get_file_information");
        free(data);
        return NULL;
    }
    
    data->extension = get_extension(filename);
    if(data->extension == NULL) {
        log_error_aux("Could not get file extension on get_file_information");
        free(data->file_content);
        free(data);
        return NULL;
    }

    return data;
}

void append_filelen_to_stream(unsigned char* stream, int filelen) {
    stream[0] = (filelen >> 24) & 0xFF;
    stream[1] = (filelen >> 16) & 0xFF;
    stream[2] = (filelen >> 8) & 0xFF;
    stream[3] = filelen & 0xFF;
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
    if(stream == NULL){
        log_error_aux("Allocation of memory failed on concatenate");    
        return NULL;
    }

    // filelen | file content | extension
    append_filelen_to_stream(stream, data->filelen);
    append_file_content_to_stream(stream, data);
    appeend_extension_to_stream(stream, data);

    return stream;
}

int get_filelen_from_stream(unsigned char* stream) {
    int filelen = (int)stream[3] | ( (int)stream[2] << 8 ) | ( (int)stream[1] << 16 ) | ( (int)stream[0] << 24 );;
    printf("filelen obtained: %d\n", filelen);
    return filelen;
}

char* get_file_content_from_stream(int filelen, unsigned char* stream) {
    char* content = malloc(sizeof(unsigned char)*filelen);
    if(content == NULL) {
        log_error_aux("Allocation of memory failed on get_file_content_from_stream");
        return NULL;
    }

    for(int i=0; i<filelen; i++) {
        content[i] = (char)stream[DWORD_SIZE + i];
    }
    return content;
}

char* get_extension_from_stream(int filelen, unsigned char* stream) {
    char* extension = malloc(MAX_EXTENSION_LEN*sizeof(char));
    if(extension == NULL){
        log_error_aux("Allocation of memory failed on get_extension_from_stream");
        return NULL;
    }

    int i = 0;
    while(stream[DWORD_SIZE + filelen + i]!='\0') {
        extension[i] = (char)stream[DWORD_SIZE + filelen + i];
        i++;
    }
    extension[i] = '\0';
    return extension;
}

/* stream:   extracted when running a lsb algorithm
   filename: for creating the file
*/
file_data* split(unsigned char* stream, int height, int width) {
    file_data* data    = malloc(sizeof(*data));
    if(data == NULL){
        log_error_aux("Allocation of memory failed on split");
        return NULL;
    }
    
    data->filelen      = get_filelen_from_stream(stream);
    if(data->filelen < 0 || data->filelen > (height*width)) {
        log_error_aux("Invalid file length on split");
        free(data);
        return NULL;
    }

    data->file_content = get_file_content_from_stream(data->filelen, stream);
    data->extension    = get_extension_from_stream(data->filelen, stream);

    return data;
}

// output_file_name MUST NOT include extension
int generate_output_file(file_data* data, char* output_file_name) {

    int file_name_len = strlen(output_file_name);
    int len = file_name_len + MAX_EXTENSION_LEN;
    
    char* file = malloc(sizeof(char)*len);
    if(file == NULL){
        log_error_aux("Allocation of memory failed on generate_output_file");
        return FAILURE;
    }
    
    int i = 0;
    for(; i<file_name_len; i++) {
        file[i] = output_file_name[i];
    }
    int j = 0;
    while(data->extension[j] != '\0') {
        file[i++] = data->extension[j++]; 
    }
    file[i] = '\0';

    printf("file name:%s\n",file);

    FILE* output = fopen(file, "w");
    if(output == NULL) {
        log_error_aux("File opening failed on generate_output_file");
        return FAILURE;
    }
    
    fwrite(data->file_content,data->filelen, 1,output);
    fclose(output);
    
    return SUCCESS;
}

void free_file_data(file_data* data) {
    if(data == NULL) return;

    free(data->file_content);
    free(data->extension);
    free(data);
}
