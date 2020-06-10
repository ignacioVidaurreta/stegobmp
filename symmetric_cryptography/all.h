#ifndef ALL_H
#define ALL_H


unsigned char* des_ecb_encrypt(char* password, unsigned char* stream, long stream_length);
unsigned char* des_ecb_decrypt(char* password, unsigned char* encrypted_stream);
unsigned char* des_cbc_encrypt(char* password, unsigned char* stream, long stream_length);
unsigned char* des_cbc_decrypt(char* password, unsigned char* encrypted_stream);

#endif /*   ALL_H */