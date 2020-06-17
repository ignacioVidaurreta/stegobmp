#ifndef LSBI_H
#define LSBI_H
    
    int embed_lsbi(const unsigned char* stream, int stream_size, pixel*** image, int width, int height, int hop);
    long extract_data_size(pixel*** image, int width, int height, int hop);
    int calculate_extension_size(pixel*** image, int width, int height, long data_sizem, int hop);
    unsigned char* extract_lsbi(pixel*** image, int width, int height, int hop);
    int run_lsbi_embed(information* info, const unsigned char* stream, long stream_size);
    unsigned char* run_lsbi_extract(information* info);

#endif /* LSBI_H */