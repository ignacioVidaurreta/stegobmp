#ifndef FILE_ANALYZER_H
#define FILE_ANALYZER_H

typedef struct {
    unsigned char* file_content;
    char*          extension;
    int            file_size;
} file_data;

/* Returns a stream with file content, extension and size concatenated
*/
unsigned char* concatenate(file_data* data);

/* Returns file content, extension, and size separated,
    obtained from a stream of bytes
*/
file_data* split(unsigned char* stream);




#endif /* FILE_ANALYZER_H */