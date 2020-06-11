#ifndef SYMMETRIC_CRYPTOGRAPHY_H
#define SYMMETRIC_CRYPTOGRAPHY_H


unsigned char* des_ecb_encrypt(char* password, unsigned char* stream, long stream_length);
unsigned char* des_ecb_decrypt(char* password, unsigned char* encrypted_stream);
unsigned char* des_cbc_encrypt(char* password, unsigned char* stream, long stream_length);
unsigned char* des_cbc_decrypt(char* password, unsigned char* encrypted_stream);

#endif /*   SYMMETRIC_CRYPTOGRAPHY_H */