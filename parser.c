#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/parser.h"

#define MAX_LEN 100

struct config* init_config(struct config* config){
    config = malloc(sizeof(struct config));

    config->in_file  = malloc(MAX_LEN * sizeof(char));
    config->bmp_file = malloc(MAX_LEN * sizeof(char));
    config->out_file = malloc(MAX_LEN * sizeof(char));

    return config;
}

void free_config(struct config* config){
    free(config->in_file);
    free(config->bmp_file);
    free(config->out_file);

    free(config);
}

bool is_invalid_param(int argc, char* argv[], int i){
    return i+1 == argc || argv[i+1][0] == '-';
}

int get_bmp_file(int argc, char* argv[], int i, struct config* config){
    if(is_invalid_param(argc, argv, i)){
        // TODO: Fail gracefully, error handling
        printf("Error en bmp wacho\n");
        return 0;
    }else{
        strcpy(config->bmp_file, argv[i+1]);
        printf("bmp: %s\n", config->bmp_file);
    }

    return 1;
}

int get_out_file(int argc, char* argv[], int i, struct config* config){
    if(is_invalid_param(argc, argv, i)){
        // TODO: Fail gracefully, error handling
        printf("Error en out wacho\n");
        return 0;
    }else{
        strcpy(config->out_file, argv[i+1]);
        printf("Out: %s\n", config->out_file);
    }

    return 1;
}

int set_steg_algorithm(int argc, char* argv[], int i, struct config* config){
    if(is_invalid_param(argc, argv, i)){
        //TODO: Fail gracefully, error handling
        printf("Error en set steg wacho\n");
        return 0;
    }else{
        char* steg_algo = argv[i+1];
        if(strcmp(steg_algo, "LSB1") == 0){
            config->steg_algorithm = LSB1;
        }else if (strcmp(steg_algo, "LSB4") == 0){
            config->steg_algorithm = LSB4;
        }else if(strcmp(steg_algo, "LSBI") == 0){
            config->steg_algorithm = LSBI;
        }else{
            printf("Steg algorithm invalido\n");
            return 0;
        }
    }

    return 1;
}
struct config* parse_arguments(int argc, char* argv[]){

    struct config* program_config = init_config(program_config);
    bool missing_mode = true;

    for(int i=1; i<argc; i++){
        char* param = argv[i];
        if(strcmp(param, "-embed") == 0 || strcmp(param, "-extract") == 0){
            program_config->mode = (strcmp(param, "-embed") == 0)?EMBED:EXTRACT;
            missing_mode = false;
        }else if(strcmp(param, "-p") == 0){
            if(get_bmp_file(argc, argv, i, program_config) == 0){
                return program_config;
            }
            i++; // Skip the argument of -p
        }else if(strcmp(param, "-out") == 0){
            if(get_out_file(argc, argv, i, program_config) == 0){
                return program_config;
            }
            i++; //Skip the argument of -out
        }else if(strcmp(param, "-steg") == 0){
            if(set_steg_algorithm(argc, argv, i, program_config) == 0){
                return program_config;
            }
            printf("ALG: %d\n", program_config->steg_algorithm);
            i++; // Skip the argument of -steg
        }

    }

    if(missing_mode){
        printf("Error: Must specify if you are trying to embed an image or extract an embedde image\n");
    }

    return program_config;
}