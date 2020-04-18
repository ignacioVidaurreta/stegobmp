#include <stdio.h>

#include "include/parser.h"
#include "include/rc4.h"

// https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
int main(int argc, char * argv[]){
    struct config* program_config;

    program_config = parse_arguments(argc, argv);
    
    free_config(program_config);
    //rc4();
}