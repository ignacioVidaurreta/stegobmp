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
#include "include/file.h"
#include "include/errors.h"
#include "include/print.h"
#include "include/embed.h"
#include "include/extract.h"


int main(int argc, char * argv[]){

    struct config* program_config = parse_arguments(argc, argv);
   
    if(program_config == NULL){
        log_error_aux("Aborting...");
        return PARAM_ERROR;
    }

    if (program_config->mode == EMBED){

        int embed_result = embed(program_config);

        if(embed_result != SUCCESS)
            return FAILURE;
            
    }else if( program_config->mode == EXTRACT){

        int extract_result = extract(program_config);

        if(extract_result != SUCCESS)
            return FAILURE;
    }

    log_info("Programa terminado \n\n", program_config);
    free_config(program_config);

    return 0;
}
