#ifndef SIGFOX_HMAC_H 
#define SIGFOX_HMAC_H  

#define AES_BLOCK_SIZE 16
#define KEYSIZE AES_BLOCK_SIZE
#define MAX_BUF_AES 32

//extern const unsigned char 	key[KEYSIZE];

int sfx_AES_128_cbc_encrypt(unsigned char *Encrypted_data, unsigned char *Data_To_Encrypt, unsigned char data_len, unsigned char *key, const unsigned char *iv);
int ComputeAES(unsigned char *out, unsigned char *buf, unsigned char len, unsigned char* key, unsigned char *Nb_Blk);
void CreateHmacList(unsigned long id, unsigned char *key, unsigned char *hmaclist, unsigned int incrval);

#endif /* SIGFOX_AES_H */
