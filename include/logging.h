#ifndef LOGGING_H
#define LOGGING_H

void log_error(const char* error_message, struct config* config);
void log_info(const char* info_message, struct config* config);
char* get_timestamp();
void log_parameter(parameter param, struct config* config);








#endif /* LOGGING_H */