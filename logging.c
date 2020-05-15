#include <stdio.h>
#include "include/parser.h"
#include "include/logging.h"

void log_error(const char* error_message, struct config* config){
    fprintf(config->error_log, "[ERROR] %s", error_message);
}

void log_info(const char* message, struct config* config){
    printf("{}{}{}");
    fprintf(config->info_log, "[INFO] %s", message);
}