#ifndef LSBI_H
#define LSBI_H

    int run_lsbi_embed(information* info, const unsigned char* stream, long stream_size);
    unsigned char* run_lsbi_extract(information* info, int is_encrypted);

#endif /* LSBI_H */
