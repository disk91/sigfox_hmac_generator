#include "sigfox_hmac.h"
#include "rijndael-alg-fst.h"


static const unsigned char iv[16] = {0};


int sfx_AES_128_cbc_encrypt(unsigned char *Encrypted_data, unsigned char *Data_To_Encrypt, unsigned char data_len, unsigned char *key, const unsigned char *iv)
{
	/*unsigned char i;
	for(i = 0; i < data_len; i++)
	{
		Encrypted_data[i] = Data_To_Encrypt[i];
	}*/

	unsigned char i, j, blocks;
    unsigned long  rk[44]= {0x00000000};
    unsigned char cbc[16]= {0x00};

    rijndaelKeySetupEnc(rk, key, 128);
    for (j = 0; j < 16; j++)
        cbc[j] = iv[j];

    blocks = data_len / 16;
    for (i = 0; i < blocks; i++) {
        for (j = 0; j < 16; j++)
            cbc[j] ^= Data_To_Encrypt[j+i*16];
        rijndaelEncrypt(rk,10, cbc, Encrypted_data+i*16);
        for (j = 0; j < 16; j++)
            cbc[j] = Encrypted_data[j+i*16];
    }
    return 0;
}

int ComputeAES(unsigned char *out, unsigned char *buf, unsigned char len, unsigned char* key, unsigned char *Nb_Blk) 
{
	unsigned char i, p=0;
    unsigned char in[MAX_BUF_AES] = {0};
   
   *Nb_Blk = (len / 17) + 1;
   
   /* Gestion de la taille pour faire des blocks de 16 bytes*/
    for (i=0; i< (*Nb_Blk)*16; i++) {
        in[i] = buf[p];
        p=(p==(len-1))?0:p+1;
    }
    sfx_AES_128_cbc_encrypt(out,in , (*Nb_Blk)*16, key, iv);
    return 0;
}

void CreateHmacList(unsigned long id, unsigned char *key, unsigned char *hmaclist, unsigned int incrval, int bit)
{
	unsigned int seqnum = 0;
	unsigned char frame[10] = {0};
	unsigned char nb_block;
	unsigned char outAES[32];
	
	/*Make Frame*/
	frame[2] = (id & 0x000000FF);
	frame[3] = (id & 0x0000FF00) >> 8;
	frame[4] = (id & 0x00FF0000) >> 16;
	frame[5] = (id & 0xFF000000) >> 24;
	
	for (seqnum = 0; seqnum < 4096; seqnum += incrval) {
		if ( bit == -1 ) {
		   frame[0] = (seqnum & 0x0F00) >> 8;			
		} else {
		   frame[0] = 0x80 | ((bit & 1) << 6) | ((seqnum & 0x0F00) >> 8);  // fomat is 0b1VD0 + 4 bits seqNum with V => bit value and D downlink bit
		}
		frame[1] = seqnum & 0x00FF;
		
		ComputeAES(outAES, frame, 6, &key[0], &nb_block);
		hmaclist[2*seqnum + 1] = (outAES[(nb_block-1)*16]);   
		hmaclist[2*seqnum] = outAES[(nb_block-1)*16+1];

		//printf("0x%02X%02X ",hmaclist[2*seqnum],hmaclist[2*seqnum + 1]);
		//if ((seqnum & 0xF) == 0) printf("\n");
	}
}

