

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sigfox_hmac.h"

#define DEBUG

const char	digit [16] =
{'0','1','2','3','4','5','6','7',
'8','9','A','B','C','D','E','F'};

void show_help(char *name) {
	printf(
		"NAME\n"
		"	%s - HEX Patcher for CMT2189C lowCostButton  \n"
		"SYNOPSIS\n"
		"	%s [-h] [-v] [-x inhexfile] [-o outhexfile] [-m mapfile]\n"
		"		[-i id] [-k key] [-n numOfHmac] [-j increment] [-s seqNumStart] [-b 0/1]"
		"DESCRIPTION\n"
		"	This command is used to write a new firmware in backtap\n"
		"OPTIONS\n"
		"	-h	help.\n"
		"	-v	version.\n"
		"	-i	id in HEX(8digits)\n"
		"	-k 	key in HEX(32digits)\n"
		"   -n  num of HMAC at output (decimal - 16 multiple ) \n"
		"   -j  seqnumber increment (decimal) \n"
		"   -s  seqNumber start (decimal) \n"
		"   -b  enable bit messages with given bit value."
		"EXAMPLES\n"
		"		%s -i ABCDABCD -k AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA -r\n"
		"COPYRIGHT\n"
		"	Copyright (c) 2019 by Sigfox.\n",
		name, name, name
	);
}

//unsigned char bin[4096];

int main(int argc, char *argv[])
{
	int opt;
	extern char * optarg;
	extern int optind, optopt, opterr; 
	unsigned int id = 0;
	unsigned char key[16] = {0};
	unsigned int i = 0, oldi = -2, j;
	unsigned char hmac[2*4096] = {0};
	unsigned int num = 512;
	unsigned int inc = 8;
	unsigned int seqStart = 0;
	int bit = -1;				// when -1 - message with no information
	                            // when 0 or 1 - message with bit information 0 or 1
	
	/*
	for (i = 0; i < 4096; i+=2) {
		bin[i] = 0xFF;
		bin[i + 1] = 0x3F;
	}
	bin[4095] = 0x29;
	*/

    if ( argc == 1 ) {
    	show_help(argv[0]);
    	return 0;
    }

	while((opt = getopt(argc, argv, "hx:m:o:i:k:d:n:j:s:b:")) != -1)
	{
   		switch(opt)
		{
			case 'i': 	
						sscanf(optarg, "%8X", &id);
						break;
			case 'k':	
						for(i = 0; i < 32; i +=2)
							sscanf(optarg + i, "%2X", (unsigned int*)&key[i/2]);
						break;
			case 'n':   sscanf(optarg, "%d", &num);
						num &= 0x0FF0;  // is a 16 multiplied
			            break;
			case 'j':   sscanf(optarg, "%d", &inc);
			            break;			  
			case 's':   sscanf(optarg, "%d", &seqStart);
			  			break;          
			case ':':	printf("-%c MISSING PARAM, execute %s -h \n", optopt, argv[0]); return -1;
			case '?':	printf("UNKNOWN ARG %c\n", optopt); show_help(argv[0]); return 0;
			case 'b':   sscanf(optarg, "%d", &bit);
						break;
			default:	break;
			
			
		}
	}
	
	CreateHmacList(id, key, hmac, inc, bit);

	int first=1;
	printf("const uint8_t sfx_id[4] = { 0x%02X, 0x%02X, 0x%02X, 0x%02X };\n",
			(id >> 24) & 0xFF, 
			(id >> 16) & 0xFF, 
			(id >>  8) & 0xFF, 
			(id      ) & 0xFF 			
		);
	if ( bit == -1 || bit == 0 ) {
   	   printf("const uint16_t sfx_hmacs_0[%d] = { \n",num);
	} else {
   	   printf("const uint16_t sfx_hmacs_1[%d] = { \n",num);
	}
	int offset = 2*seqStart;
	for (j = 0; j < (num/16); j++) {
		for (i = 0; i < 16; i+=1) { 
			if ( ! first ) {
				printf(", ");
			} else {
				printf("  ");
				first = 0;
			}
			printf("0x%02X%02X", hmac[offset+1], hmac[offset]);
			offset += 2*inc;
		}
		printf("\n");	
	}
	printf("};\n");
	printf("const uint16_t sfx_seqStart = %d;\n",seqStart);
    printf("const uint16_t sfx_seqStop = %d;\n",seqStart+num-1);	
}
