#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/parser.h"
#include "include/logging.h"

#define MAX_LEN 100

struct config* init_config(struct config* config){
    config = malloc(sizeof(struct config));

    config->in_file   = malloc(MAX_LEN * sizeof(char));
    config->bmp_file  = malloc(MAX_LEN * sizeof(char));
    config->out_file  = malloc(MAX_LEN * sizeof(char));
    config->password  = malloc(MAX_LEN * sizeof(char));
    config->error_log = fopen("log/stegobmp_error.log", "a");
    config->info_log  = fopen("log/stegobmp_info.log", "a");

    return config;
}

void free_config(struct config* config){
    free(config->in_file);
    free(config->bmp_file);
    free(config->out_file);
    free(config->password);

    fclose(config->error_log);
    fclose(config->info_log);

    free(config);
}

bool is_invalid_param(int argc, char* argv[], int i){
    return i+1 == argc || argv[i+1][0] == '-';
}

int get_bmp_file(char* filename, struct config* config){
    
    size_t len = strlen(filename);
    if (len >= MAX_LEN){
        return 0;
    }

    strncpy(config->bmp_file, filename, len);
    log_parameter(BMP, config);
    
    return 1;
}

int get_out_file(char* filename, struct config* config){
    
    size_t len = strlen(filename);
    if(len >= MAX_LEN){
        return 0;
    }
    strncpy(config->out_file, filename, len);
    log_parameter(OUT, config);

    return 1;
}

int set_steg_algorithm(char* steg_algo, struct config* config){

    if(strcmp(steg_algo, "LSB1") == 0){
        config->steg_algorithm = LSB1;
    }else if (strcmp(steg_algo, "LSB4") == 0){
        config->steg_algorithm = LSB4;
    }else if(strcmp(steg_algo, "LSBI") == 0){
        config->steg_algorithm = LSBI;
    }else{
        log_error("El algoritmo de Steganografiado insertado es invalido", config);
        return 0;
    }

    log_parameter(STEG, config);

    return 1;
}

int set_encrypt_algorithm(char * enc_algo, struct config* config){
    
    if(strcmp(enc_algo, "aes128") == 0){
        config->enc_algorithm = AES128;
    }else if (strcmp(enc_algo, "aes192") == 0){
        config-> enc_algorithm = AES192;
    }else if(strcmp(enc_algo, "aes256") == 0){
        config->enc_algorithm = AES256;
    }else if(strcmp(enc_algo, "des") == 0){
        config->enc_algorithm = DES;
    }else{
        log_error("Algoritmo de encripcion invalido", config);
        return 0;
    }
    
    log_parameter(ENC, config);
    return 1;
}

int set_chaining_mode(char* mode, struct config* config){
    
    if(strcmp(mode, "ECB") == 0){
        config->enc_mode = ECB;
    }else if (strcmp(mode, "CFB") == 0){
        config-> enc_mode = CFB;
    }else if(strcmp(mode, "OFB") == 0){
        config->enc_mode = OFB;
    }else if(strcmp(mode, "CBC") == 0){
        config->enc_mode = CBC;
    }else{
        log_error("Modo de encadenamiento invalido", config);
        return 0;
    }
    
    log_parameter(CHAIN, config);
    return 1;
}


int set_password(char* pass, struct config* config){
    
    size_t len = strlen(pass);
    if(len >= MAX_LEN){
        return 0;
    }

    strncpy(config->password, pass, len);

    return 1;
}

int set_file_to_embed(char* file,struct config* config ){
    size_t len = strlen(file);
    if(len >= MAX_LEN){
        log_error("El nombre del archivo supera la capacidad máxima permitida", config);
        return 0;
    }

    strncpy(config->in_file, file, len);
    log_parameter(IN, config);
    return 1;
}

int parse_param(int argc, char* argv[], int i, struct config* config, parameter given_param){
    if(is_invalid_param(argc, argv, i)){
        log_error("Parametro(s) invalido(s)", config);
        return 0;
    }

    switch(given_param){
        case BMP:
            return get_bmp_file(argv[i+1], config);
        case OUT:
            return get_out_file(argv[i+1], config);
        case STEG:
            return set_steg_algorithm(argv[i+1], config);
        case ENC:
            return set_encrypt_algorithm(argv[i+1], config);
        case CHAIN:
            return set_chaining_mode(argv[i+1], config);
        case PASS:
            return set_password(argv[i+1], config);
        case IN:
            return set_file_to_embed(argv[i+1], config);
        default:
            return 0;
    }

    return 0;
}

void init_log_message(char* program_name, int arg_num, struct config* program_config){
    char format_info[100] = {'\0'};
    if( arg_num != 1){
        sprintf(format_info, "Running %s with %d arguments", program_name, arg_num);
    }else{
        sprintf(format_info, "Running %s with %d argument", program_name, arg_num);
    }

    log_info(format_info, program_config);
}

struct config* parse_arguments(int argc, char* argv[]){

    struct config* program_config = init_config(program_config);
    init_log_message(argv[0], argc, program_config);
    bool missing_mode = true;
    for(int i=1; i<argc; i++){
        parameter mode;
        char* param = argv[i];
        if(strcmp(param, "-embed") == 0 || strcmp(param, "-extract") == 0){
            mode = MODE;
            program_config->mode = (strcmp(param, "-embed") == 0)?EMBED:EXTRACT;
            missing_mode = false;
        }else if(strcmp(param, "-p") == 0){
            mode = BMP;
        }else if(strcmp(param, "-out") == 0){
            mode = OUT;
        }else if(strcmp(param, "-steg") == 0){
            mode = STEG;
        }else if(strcmp(param, "-a") == 0){
            mode = ENC;
        }else if(strcmp(param, "-m") == 0){
            mode = CHAIN;
        }else if(strcmp(param, "-pass") == 0){
            mode = PASS;
        }else if(strcmp(param, "-in") == 0){
            mode = IN;
        }
        if(mode != MODE){
            if(parse_param(argc, argv, i, program_config, mode) == 0){
            return program_config;
            }
            i++; // Skip the param value (not valid for MODE)
        }
        

    }

    if(missing_mode){
        printf("[Error] Must specify if you are trying to embed an image or extract an embedde image\n");
        log_error("Must specify if you are trying to embed an image or extract an embedde image\n", program_config);
        
    }

    return program_config;
}


int get_mode(int argc, char** argv){
    for(int i=0;  i<argc; i++){
        char* param = argv[i];
        if(strcmp(param, "-embed") == 0 ){
            return EMBED;
        }else if(strcmp(param, "-extract") == 0){
            return EXTRACT;
        }
    }

    return -1;

}