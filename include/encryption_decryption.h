#ifndef ENCRYPTION_DECRYPTION_H
#define ENCRYPTION_DECRYPTION_H

#define ENCRYPT 0
#define DECRYPT 1

unsigned char* run_cipher_process(char* a, char* m, char* password, int operation, unsigned char* stream);


#endif /*   ENCRYPTION_DECRYPTION_H */