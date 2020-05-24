#ifndef LSB1_H
#define LSBH1_H

void run_lsb1(information* info, unsigned char* data_stream);
void run_lsb1_embed(information* info, const unsigned char* data, long data_size);
void run_lsb1_extract(information* info, const unsigned char* data);


#endif /* LSB1_H */