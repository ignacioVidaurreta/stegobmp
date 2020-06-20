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
#include "../include/cryptography.h"
#include "../include/encryption_decryption.h"
#include "../include/embed.h"
#include "../include/extract.h"

int extract(struct config* program_config) {
    
    //print_config(program_config);

    //bmp processing
    information* info;
    info = bmp_to_matrix(program_config->bmp_file);
    
    // validate encryption
    int is_encrypted = validate_encryption(program_config);
    
    // extract. TODO: error handling
    unsigned char* stream = run_extract_algorithm(program_config, info, is_encrypted);
    if(stream == NULL)
        return ERROR_EXTRACT;
    
    // decrypt if necessary
    if (is_encrypted) {
        int len = get_len_from_stream(stream);
        cipher_info* dec_info = run_cipher_process(program_config->enc_algorithm, program_config->enc_mode, program_config->password, DECRYPT, stream + DWORD, len, FALSE);
        if(dec_info == NULL)
            return ERROR_DECRYPTION;
        stream = dec_info->output_stream;
    }

    // split information after running lsb
    file_data* split_data = split(stream);
    int result = generate_output_file(split_data, program_config->out_file);
    if(split_data == NULL)
        return ERROR_SPLIT;
    if(result != SUCCESS)
        return result;

    free(stream);
    free_file_data(split_data);
    free_config(program_config);
    free_information(info);

    return SUCCESS;
}

// Checks optional parameters and selects default values for decryption
// algorithm and/or mode if necessary. If password is NULL encryption
// must not happen
int validate_encryption(struct config* program_config) {
   
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

unsigned char* run_extract_algorithm(struct config* program_config, information* info, int is_encrypted) {
    
    switch(program_config->steg_algorithm) {
        case LSB1:
            return run_lsb1_extract(info, is_encrypted);
        case LSB4:
            return run_lsb4_extract(info, is_encrypted);
        case LSBI:
            return run_lsbi_extract(info);
            break;
        default:
            return NULL;
    }
}
