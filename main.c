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
#include "include/base.h"


// https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
int main(int argc, char * argv[]){

    
    char * mode = "TXT";

    if(strcmp(mode, "TXT") == 0){
        // If this file isn't in your filesystem please create it
        char* filename = "dummy.txt";
        struct config* program_config = parse_arguments(argc, argv);
        
        char* img = get_byte_repr(filename);
        char* ext = get_extension(filename, program_config);
        free(img);
        log_info("Programa terminado \n\n", program_config);
        free_config(program_config);
        return 0;
    }

    information* info = bmp_to_matrix(argv[1]);

    if(strcmp(mode, "LSBI") == 0){
        run_lsbi(info);
    }else if(strcmp(mode, "RC4") == 0){
        rc4();
    }else if(strcmp(mode, "LSB1") == 0){
        run_lsb1(info);
    }else if(strcmp(mode, "LSB4") == 0){
        run_lsb4(info);
    }


  
}