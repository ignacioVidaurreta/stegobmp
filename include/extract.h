#ifndef EXTRACT_H
#define EXTRACT_H

int extract(struct config* program_config);
int validate_encryption(struct config* program_config);
unsigned char* run_extract_algorithm(struct config* program_config, information* info);

#endif
