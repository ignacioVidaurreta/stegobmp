#ifndef PARSER__H
#define PARSER__H

// TODO: Transform into an enum
#define EMBED 1
#define EXTRACT 0

//TODO: Transform into an enum
#define LSB1 0
#define LSB4 1
#define LSBI 2


typedef enum {AES128, AES192, AES256, DES } enc_alg;

typedef enum {ECB, CFB, OFB, CBC} chain_mode;

typedef enum {MODE, BMP, OUT, STEG, ENC, CHAIN, PASS} parameter;
#include <stdbool.h>

struct config{
    int mode;

    char * in_file;
    char * bmp_file;
    char * out_file;
    char* password;

    int steg_algorithm;
    enc_alg enc_algorithm;
    chain_mode enc_mode;

    FILE* error_log;
    FILE* info_log;
};

struct config* init_config(struct config* config);
void free_config(struct config* config);

struct config * parse_arguments(int argc, char* argv[]);
bool is_invalid_param(int argc, char* argv[], int i);
int get_bmp_file(char* filename, struct config* config);
int get_out_file(char* filename, struct config* config);
int set_steg_algorithm(char* steg_algo, struct config* config);
int set_encrypt_algorithm(char* enc_algo, struct config* config);
int set_chaining_mode(char* mode, struct config* config);
int set_password(char* pass, struct config* config);
int get_mode(int argc, char** argv);



#endif