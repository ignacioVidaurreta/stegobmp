#ifndef ENCRYPTION_DECRYPTION_H
#define ENCRYPTION_DECRYPTION_H

#define ENCRYPT 0
#define DECRYPT 1

typedef struct cipher_info{
    unsigned char* output_stream;
    int output_len;
}cipher_info;

/* Runs cipher process for both encryption and decryption

***Arguments***
a          : algorithm, MUST BE lower case
m          : mode, MUST BE lower case
password   : for encryption/decryption, MUST NOT BE null
operation  : ENCRYPT or DECRYPT
stream     : stream to encrypt or decrypt
stream_len : self explanatory
append_len : if we are running an encryption, this will append the legnth of cipher at the beginning

***Returns***

cipher info structure with output_stream and output_len 
(notice that if we set append_len = TRUE, output_stream has output_len appended at the beginning)

*/
cipher_info* run_cipher_process(
    char* a, char* m, char* password, int operation, unsigned char* stream, int stream_len, int append_len);


#endif /*   ENCRYPTION_DECRYPTION_H */