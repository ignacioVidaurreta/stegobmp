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

int embed(struct config* program_config) {
    
    // file processing. TODO: error handling
    char* filename = program_config->in_file;
    file_data*     data       = get_file_information(filename);
    unsigned char* stream     = concatenate(data);

    // bmp processing. TODO: error handling
    information* info;
    info = bmp_to_matrix(program_config->bmp_file);
    
    // stream size
    long stream_size = DWORD + data->filelen + strlen(data->extension) + 1;

    // encryption. TODO: error handling
    cipher_info* enc_info = NULL;
    int do_encryption = validate_encryption_intention(program_config);
    if(do_encryption)
        enc_info = run_cipher_process(program_config->enc_algorithm, program_config->enc_mode, program_config->password, ENCRYPT, stream, stream_size, TRUE);
 
    // embed
    int steg_result = FAILURE;
    if(do_encryption)
        steg_result = run_embed_algorithm(program_config, info, (const unsigned char*) enc_info->output_stream, enc_info->output_len + DWORD);
    else
        steg_result = run_embed_algorithm(program_config, info,(const unsigned char*) stream, stream_size);
    
    if(steg_result != SUCCESS)
        return steg_result;

    // bmp processing
    int matrix_to_bpm_result = matrix_to_bmp(info, program_config->out_file);

    if(matrix_to_bpm_result != SUCCESS)
        return matrix_to_bpm_result;
        
    // memory release
    free(stream);
    free_file_data(data);
    free_config(program_config);
    free_information(info);
    free_cipher_info(enc_info);

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
