#ifndef ENCRYPTION_DECRYPTION_H
#define ENCRYPTION_DECRYPTION_H

#define ENCRYPT 0
#define DECRYPT 1

#include "parser.h"

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

***Example***

We want to encrypt stream="hola que tal" with aes128, ecf mode and we want to append len at the beginning.
stream="hola que tal";
enc_alg a = 0;
chain_mode m = 0;
int stream_len = strlen(stream)+1;
cipher_info enc_info = run_cipher_process(a, m, "pass", ENCRYPT, stream, stream_len, TRUE);

Now, we want to decrypt.
int len = get_len_from_stream(enc_info->output_stream); // we need to include cryptography.h
cipher_info dec_info = run_cipher_process(a, m, "pass", DECRYPT, enc_info->output_stream + 4, len, FALSE);

printf("%s",dec_info->output_stream) == "hola que tal"

*/
cipher_info* run_cipher_process(
    enc_alg a, chain_mode m, char* password, int operation, unsigned char* stream, int stream_len, int append_len);


#endif /*   ENCRYPTION_DECRYPTION_H */