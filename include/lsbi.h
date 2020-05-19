#ifndef LSBI_H
#define LSBHI_H

    static int embed_message(int i, pixel* pixel, int data_size, unsigned char* data);
    static void embed(unsigned char* data, int data_size, pixel*** image, int width, int height);
    static int extract_data(int i, pixel* pixel, int data_size, char* data);
    static unsigned char* extract(pixel*** image, int width, int height);
    
    int run_lsbi(information* info);

#endif /* LSBI_H */