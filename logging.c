#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "include/parser.h"
#include "include/logging.h"

void log_error(const char* error_message, struct config* config){
    char* timestamp = get_timestamp();
    fprintf(config->error_log, "[%s][ERROR] %s\n", timestamp, error_message);
    free(timestamp);
}

void log_info(const char* message, struct config* config){
    char* timestamp = get_timestamp();
    fprintf(config->info_log, "[%s][INFO] %s\n", timestamp, message);
    free(timestamp);
}

char* get_timestamp(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char * timestamp = malloc(10*sizeof(char));
    sprintf(timestamp, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

    return timestamp;
}

char* get_steg_name(int steg_algorithm){
    switch(steg_algorithm){
        case LSB1:
            return "LSB1";
        case LSB4:
            return "LSB4";
        case LSBI:
            return "LSBI";
        default:
            return "Unkown";
    }

}

char* get_enc_name(enc_alg enc_algorithm){
    switch(enc_algorithm){
        case AES128:
            return "AES128";
        case AES192:
            return "AES192";
        case AES256:
            return "AES256";
        case DES:
            return "DES";
        default:
            return "Unknown";
    }
}

char* get_chain_alg_name(chain_mode mode){
    switch(mode){
        case ECB:
            return "ECB";
        case CFB:
            return "CFB";
        case OFB:
            return "OFB";
        case CBC:
            return "CBC";
        default:
            return "Unknown";
    }
}

void log_parameter(parameter param, struct config* config){

    char* timestamp = get_timestamp();
    char* message = malloc(100*sizeof(char));
    switch(param){
        case BMP:
            sprintf(message, "Nombre del archivo BMP a usar: %s", config->bmp_file);
            break;
        case OUT:
            sprintf(message, "Nombre del archivo que contiene el output: %s", config->out_file);
            break;
        case STEG:
            sprintf(message, "Nombre del algoritmo de steganografia a usar: %s",
                    get_steg_name(config->steg_algorithm));
            break;
        case ENC:
            sprintf(message, "Nombre del algoritmo de encripcion a usar: %s",
                    get_enc_name(config->enc_algorithm));
            break;
        case CHAIN:
            sprintf(message, "Nombre del algoritmo de encadenamiento a usar: %s",
                    get_chain_alg_name(config->enc_algorithm));
            break;
        default:
            log_error("Unknown parameter", config);
            return;
    }

    log_info(message, config);

    free(timestamp);
    free(message);

}

void log_extension(char* ext, bool error, struct config* config){
    char* message = malloc(100*sizeof(char));

    if(error){
        sprintf(message, "Error: %s file type is unsupported", ext);
        log_error(message, config);
    }else{
        sprintf(message, "Using extension: %s", ext);
        log_info(message, config);
    }

    free(message);
}