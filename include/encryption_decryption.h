#ifndef ENCRYPTION_DECRYPTION_H
#define ENCRYPTION_DECRYPTION_H

#define ENCRYPT 0
#define DECRYPT 1

typedef struct cipher_info{
    unsigned char* output_stream;
    int output_len;
}cipher_info;

cipher_info* run_cipher_process(
    char* a, char* m, char* password, int operation, unsigned char* stream, int stream_len, int append_len);


#endif /*   ENCRYPTION_DECRYPTION_H */