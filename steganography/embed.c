#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/logging.h"
#include "../include/bmp.h"
#include "../include/lsbi.h"
#include "../include/lsb1.h"
#include "../include/lsb4.h"
#include "../include/file.h"
#include "../include/errors.h"
#include "../include/print.h"
#include "../include/steganography.h"
#include "../include/encryption_decryption.h"
#include "../include/embed.h"

static int VERBOSE = 1;

int embed(struct config* program_config) {
    
    if(VERBOSE) printf("Starting embedding process...\n");
    // file processing
    char* filename = program_config->in_file;
    file_data*     data       = get_file_information(filename);
    if(data == NULL)
        return ERROR_GET_FILE_INFO;
    if(VERBOSE) printf("File information ready\n");
    
    unsigned char* stream     = concatenate(data);
    if(stream == NULL) {
        free(data);
        return ERROR_CONCATENATE;
    }
    if(VERBOSE) printf("Stream prepared to embed\n");

    // bmp processing
    information* info;
    info = bmp_to_matrix(program_config->bmp_file);
    if(info == NULL) {
        free(data);
        free(stream);
        return ERROR_BMP_TO_MATRIX;
    }
    if(VERBOSE) printf("BMP matrix prepared to embed\n");

    // stream size
    long stream_size = DWORD + data->filelen + strlen(data->extension) + 1;

    // encryption
    cipher_info* enc_info;
    int do_encryption = validate_encryption_intention(program_config);
    if(do_encryption) {
        enc_info = run_cipher_process(program_config->enc_algorithm, program_config->enc_mode, program_config->password, ENCRYPT, stream, stream_size, TRUE);
        if(enc_info == NULL) {
            free(data);
            free(stream);
            free(info);
            return ERROR_ENCRYPTION;
        }
        if(VERBOSE) printf("Stream encrypted\n");
    }
 
    // embed
    int steg_result = FAILURE;
    if(do_encryption)
        steg_result = run_embed_algorithm(program_config, info, (const unsigned char*) enc_info->output_stream, enc_info->output_len + DWORD);
    else
        steg_result = run_embed_algorithm(program_config, info,(const unsigned char*) stream, stream_size);
    
    if(steg_result != SUCCESS) {
        free(data);
        free(stream);
        free(info);
        return steg_result;
    }
    if(VERBOSE) printf("Stream successfully embedded\n");

    // bmp processing
    int matrix_to_bpm_result = matrix_to_bmp(info, program_config->out_file);

    if(matrix_to_bpm_result != SUCCESS)
        return matrix_to_bpm_result;

    // memory release
    free(stream);
    free_file_data(data);
    free_information(info);
    if(VERBOSE) printf("Deallocation of memory completed\n");

    return SUCCESS;
}


int run_embed_algorithm(struct config* program_config, information* info, const unsigned char* stream, long stream_size) {
    
    int steg_result = ERROR_SIZE;
    
    switch(program_config->steg_algorithm) {
        case LSB1:
            steg_result = run_lsb1_embed(info, stream, stream_size);
            break;
        case LSB4:
            steg_result = run_lsb4_embed(info, stream, stream_size);
            break;
        case LSBI:
            steg_result = run_lsbi_embed(info, (const unsigned char*) stream, stream_size);
            break;
        default:
            steg_result = PARAM_ERROR;
    }
   
   return steg_result;
}


// Checks optional parameters and selects default values for encryiption
// algorithm and/or mode if necessary. If password is NULL encryption
// must not happen
int validate_encryption_intention(struct config* program_config) {
   
    if(*program_config->password == '\0')
        return FALSE;

    if(program_config->enc_algorithm == EMPTY) {
        program_config->enc_algorithm = AES128;
    }
    if (program_config->enc_mode == EMPTY) {
        program_config->enc_mode = CBC;
    }

    return TRUE;
}
