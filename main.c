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
#include "include/errors.h"

void run_with_errors(int argc, char** argv){
     // TODO: desconectamos get_file_information de program_config y dejamos
    // el llamado a funcion de parse_arguments y asi y todo nos daban mal 
    // las longitudes del archivo en store_byte_repr_and_size
    // lo raro es que seguia pasando cuando unicamente estabamos inicializando 
    // la config (ya no la estabamos usando entre los metodos que involucran buscar la file information).
    char* filename = "dummy.txt";
        
    // when running this, filelen=454, len=458 (len of file_content, the one we write)
    struct config* program_config = parse_arguments(argc, argv);
    char* ext = get_extension(filename, program_config);
    free_config(program_config);
}
// https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
int main(int argc, char * argv[]){

    bool postmortem = true;
    if(postmortem){
       run_with_errors(argc, argv);
    }
    
    // If this file isn't in your filesystem please create it
    char* filename = "dummy.txt";

    information* info;

    int mode = get_mode(argc, argv);
    if (mode == EMBED){

        file_data*     data       = get_file_information(filename);
        unsigned char* stream     = concatenate(data);
        
        long stream_size = 4 + data->filelen + strlen(data->extension) + 1;

        info = bmp_to_matrix("./images/ladoLSB1.bmp");

        printf("bmp_to_matrix done\n");

        // if embed_result is ERROR_SIZE, the stream size exceeds the available space in the image for lsb1 
        int embed_result = run_lsb1_embed(info, (const unsigned char*) stream, stream_size);
        //TODO: improve error message. This error is being logged but we need to show it also in stdout
        if(embed_result == ERROR_SIZE)
            printf("[ERROR] Can't embed image: size exceeds available space");

        printf("embeded done with result = %d\n", embed_result);

        int result = matrix_to_bmp(info, "testfile.bmp");

        printf("matrix_to_bmp done with result = %d\n", embed_result,result);

        return SUCCESS;
    }else if( mode == EXTRACT){

        info = bmp_to_matrix("testfile.bmp");

        printf("matrix ready to be extracted from\n");

        unsigned char* stream = run_lsb1_extract(info); //this must be freed at the end

        printf("extraction accomplished\n");

        // split information after running lsb
        file_data*     split_data = split(stream);
        int result                = generate_output_file(split_data, "output_test");
        printf("result of output file: %d\n", result);

        return SUCCESS;
    }


    // log_info("Programa terminado \n\n", program_config);
    // free_config(program_config);
    
    // free(data);
    // free(split_data);
    return 0;
}