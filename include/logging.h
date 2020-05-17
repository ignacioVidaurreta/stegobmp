#ifndef LOGGING_H
#define LOGGING_H

void log_error(const char* error_message, struct config* config);
void log_info(const char* info_message, struct config* config);
char* get_timestamp();
void log_parameter(parameter param, struct config* config);
char* get_chain_alg_name(chain_mode mode);
char* get_enc_name(enc_alg enc_algorithm);
char* get_steg_name(int steg_algorithm);









#endif /* LOGGING_H */