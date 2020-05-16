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

void log_parameter(parameter param, struct config* config){

    char* timestamp = get_timestamp();
    char* message = malloc(100*sizeof(char));
    switch(param){
        case BMP:
            sprintf(message, "Nombre del archivo BMP a usar: %s", config->bmp_file);
            log_info(message, config);
            break;
        default:
            log_error("Unknown parameter", config);
    }

    free(timestamp);
    free(message);

}