#include <stdio.h>

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
    //rc4();
     information* info = bmp_to_matrix(argv[1]);
     run_lsb4(info);
     printf("My matrix has width %d and %d height\n", info->width, info->height);
    // log_info("Programa terminado \n\n", program_config);
    //free_config(program_config);
    //rc4();
    
    //run_lsbi();
}