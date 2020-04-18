#ifndef PARSER__H
#define PARSER__H

// TODO: Transform into an enum
#define EMBED 1
#define EXTRACT 0

//TODO: Transform into an enum
#define LSB1 0
#define LSB4 1
#define LSBI 2

#include <stdbool.h>

struct config{
    int mode;

    char * in_file;
    char * bmp_file;
    char * out_file;

    int steg_algorithm;
};

struct config* init_config(struct config* config);
void free_config(struct config* config);

struct config * parse_arguments(int argc, char* argv[]);
bool is_invalid_param(int argc, char* argv[], int i);
int get_bmp_file(int argc, char* argv[], int i, struct config* config);
int get_out_file(int argc, char* argv[], int i, struct config* config);
int set_steg_algorithm(int argc, char* argv[], int i, struct config* config);

#endif