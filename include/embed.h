#ifndef EMBED_H
#define EMBED_H

#define TRUE 1
#define FALSE 0

int embed(struct config* program_config);
int run_embed_algorithm(struct config* program_config, information* info, const unsigned char* stream, long stream_size);
int validate_encryption_intention(struct config* program_config);

#endif