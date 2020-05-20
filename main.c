#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/parser.h"
#include "include/logging.h"
#include "include/rc4.h"
#include "include/bmp.h"
#include "include/lsbi.h"
#include "include/lsb1.h"
#include "include/lsb4.h"


// https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
int main(int argc, char * argv[]){
    //struct config* program_config;

    //program_config = parse_arguments(argc, argv);
    
    //free_config(program_config);
    information* info = bmp_to_matrix(argv[1]);
    char * mode = "LSBI";

    if(strcmp(mode, "LSBI") == 0){
        run_lsbi(info);
    }else if(strcmp(mode, "RC4") == 0){
        rc4();
    }else if(strcmp(mode, "LSB1") == 0){
        run_lsb1(info);
    }else if(strcmp(mode, "LSB4") == 0){
        run_lsb4(info);
    }


    // log_info("Programa terminado \n\n", program_config);
    //free_config(program_config);
}