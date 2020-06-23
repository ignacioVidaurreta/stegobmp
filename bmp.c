#include <stdio.h>
#include <stdlib.h>

#include "include/bmp.h"
#include "include/errors.h"
#include "include/logging.h"

#define HEADER_SIZE 54

void free_header(bmp_header* header) {
    free(header);
}

void free_pixel_matrix(pixel*** matrix, unsigned int height, unsigned int width) {
    for(int i=0; i<height; i++) {
        for(int j=0; j<width;j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
}

void free_information(information* info) {
    free_pixel_matrix(info->matrix, info->header->bmp_height, info->header->bmp_width);
    free_header(info->header);
    free(info);
}

information* bmp_to_matrix(const char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    if(file == NULL) {
        log_error_aux("File opening failed on bmp_to_matrix");
        return NULL;
    }

    bmp_header* header = malloc(sizeof(*header));
    if(header == NULL) {
        log_error_aux("Memory allocation failed on bmp_t_matrix (header)");
        fclose(file);
        return NULL;
    }

    fread(header,sizeof(*header),1,file);

    unsigned int height = header->bmp_height;
    unsigned int width = header->bmp_width;

    pixel*** matrix = malloc(sizeof(*matrix)*height);
    if(matrix == NULL) {
        log_error_aux("Memory allocation failed on bmp_t_matrix (matrix)");
        fclose(file);
        free(header);
        return NULL;
    }

    for(int i=0; i<height; i++) {
        matrix[i] = malloc(sizeof(**matrix)*width);
        for(int j=0; j<width; j++) {
            matrix[i][j] = malloc(sizeof(pixel));
        }
    }
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            fread(matrix[i][j],sizeof(pixel),1,file);
        }
    }
    information* info = malloc(sizeof(*info));
    if(info == NULL) {
        free_header(header);
        free_pixel_matrix(matrix, header->bmp_height, header->bmp_width);
        fclose(file);
        log_error_aux("Memory allocation failed on bmp_t_matrix (info)");
        return NULL;
    }
    info->matrix = matrix;
    info->header = header;
    fclose(file);
    return info;
}

/* file_name: must contain .bmp extension */
int matrix_to_bmp(information* info, char* file_name) {
    FILE* bmp_file = fopen(file_name, "w");
    if(bmp_file == NULL) {  
        log_error_aux("File opening failed on matrix_to_bmp");              // Problems when opening the file...
        return FAILURE;
    }
    unsigned int result = fwrite(info->header, HEADER_SIZE, 1, bmp_file);   // Writes header into file
    if(result != 1) {                                                       // If it didn't write what it needed to write...
        log_error_aux("fwrite failed on matrix_to_bmp");
        return FAILURE;
    }
    unsigned int height = info->header->bmp_height;
    unsigned int width  = info->header->bmp_width;
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            result = fwrite(info->matrix[i][j],sizeof(pixel), 1,bmp_file);  // Writes every pixel into file
            if(result != 1) {                                               // If it didn't write what it needed to write...
                log_error_aux("fwrite failed on matrix_to_bmp loop");
                return FAILURE;
            }
        }
    }
    fclose(bmp_file);
    return SUCCESS;
}