#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/logging.h"
#include "../include/rc4.h"
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

static void print_config(struct config* program_config) {
    printf("in_file %s\n",program_config->in_file);
    printf("bmp_file %s\n",program_config->bmp_file);
    printf("out_file %s\n",program_config->out_file);
    printf("steg_algorithm %d\n",program_config->steg_algorithm);
    printf("enc_algorithm %d\n",program_config->enc_algorithm);
    printf("enc_mode %d\n",program_config->enc_mode);
}

int extract(struct config* program_config) {
    
    print_config(program_config);

    //bmp processing
    information* info;
    info = bmp_to_matrix(program_config->bmp_file);

    // validate encryption
    int is_encrypted = validate_encryption(program_config);

    // extract. TODO: error handling
    unsigned char* stream = run_extract_algorithm(program_config, info);

    // split information after running lsb
    file_data*     split_data = split(stream);
    int result                = generate_output_file(split_data, program_config->out_file);

    free(stream);
    free_file_data(split_data);
    free_config(program_config);
    free_information(info);

    return SUCCESS;
}

int validate_encryption(struct config* program_config) {
    return FALSE;
}

unsigned char* run_extract_algorithm(struct config* program_config, information* info) {
    
    int steg_algorithm = program_config->steg_algorithm;
    
    switch(steg_algorithm) {
        case LSB1:
            return run_lsb1_extract(info);
        case LSB4:
            return run_lsb4_extract(info);
        case LSBI:
            //return run_lsbi_extract(info);
            break;
    }
}