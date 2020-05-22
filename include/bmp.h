#ifndef BMP_H
#define BMP_H

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} pixel;

// unsigned short --> size 2
// unsigned int   --> size 4
typedef struct __attribute__((__packed__)) {
    unsigned short type;
    unsigned int bmp_size;
    unsigned short reserved_1;
    unsigned short reserved_2;
    unsigned int bmp_data_offset;
    unsigned int bmp_header_size;
    unsigned int bmp_width;
    unsigned int bmp_height;
    unsigned short plane_number;
    unsigned short point_size;
    unsigned int compression;
    unsigned int image_size;
    unsigned int horizontal_resolution;
    unsigned int vertical_resolution;
    unsigned int colour_table_size;
    unsigned int important_colour_counter;
} bmp_header;

typedef struct {
    pixel***     matrix;
    bmp_header*  header;
} information;

information* bmp_to_matrix(const char* filename);

/* file_name: must contain .bmp extension on the name.
    Creates an output file on root folder containing the image */
int matrix_to_bmp(information* info, char* file_name);

void free_information(information* info);


#endif /* BMP_H */