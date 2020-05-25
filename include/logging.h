#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include "parser.h"

void log_error_aux(const char* error_message);
void log_info_aux(const char* message, struct config* config);
void log_error(const char* error_message, struct config* config);
void log_info(const char* info_message, struct config* config);
char* get_timestamp();
void log_parameter(parameter param, struct config* config);
void log_extension(char* ext, bool error, struct config* config);
char* get_chain_alg_name(chain_mode mode);
char* get_enc_name(enc_alg enc_algorithm);
char* get_steg_name(int steg_algorithm);









#endif /* LOGGING_H */