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
}
// https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
int main(int argc, char * argv[]){

    bool postmortem = false;
    if(postmortem){
       run_with_errors(argc, argv);
    }
    char * mode = "TXT";
    
    if(strcmp(mode, "TXT") == 0){
        // If this file isn't in your filesystem please create it
        char* filename = "dummy.txt";

        int mode = get_mode(argc, argv);
        if (mode == EMBED){
            printf("embed");
            return 1;
        }else if( mode == EXTRACT){
            printf("extract");
            return 1;
        }


        printf("ERROR");
        
        file_data*     data       = get_file_information(filename);
        unsigned char* stream     = concatenate(data);
        
        // stream to bits
        long stream_size = 4 + data->filelen + strlen(data->extension) + 1;
        // size of stream in bits
        long data_size = (4 + data->filelen + strlen(data->extension) + 1)*8;
        unsigned char* bit_stream = (unsigned char *)malloc(sizeof(unsigned char)*data_size);
        int j=0;
        for(int i=0; i < stream_size; i++) {
            uchar_to_byte(bit_stream+j, stream[i]);
        }

        // split information after running lsb
        file_data*     split_data = split(stream);
        int result                = generate_output_file(split_data, "output_test");
        printf("Result of output file: %d\n", result);

        // log_info("Programa terminado \n\n", program_config);
        // free_config(program_config);
        
        free(data);
        free(split_data);
        return 0;
    }
}