#include <stdio.h>
#include <stdlib.h>
#include "psteg.h"
#include "string.h"
#include "math.h"

void lsbextract(char* filename, char* orderstring, char nbit) {
	int width, height, depth;
	unsigned char *data = stbi_load(filename, &width, &height, &depth, 0);

	int i, j, k, chan_num;
	unsigned char byte;
	char ch = 0;
	int bit_counter = 0;
	unsigned char pow2 = 0x80;
	FILE *fd;
	
	chan_num = strnlen(orderstring, 4);
	
	fd = fopen(orderstring, "w");
	
	long pixels = width*height;
	for(i=0; i!=pixels; i++) {
		for(j=0; j!=chan_num; j++) {
			if(orderstring[j] == 'R' || orderstring[j] == 'r')
				byte = data[i*4];
			else if(orderstring[j] == 'G' || orderstring[j] == 'g')
				byte = data[(i*4)+1];
			else if(orderstring[j] == 'B' || orderstring[j] == 'b')
				byte = data[(i*4)+2];
			else if(orderstring[j] == 'A' || orderstring[j] == 'a')
				byte = data[(i*4)+3];
			
			for(k=nbit-1; k>=0; k--) {
				if(byte & (unsigned char)pow(2, k))
					ch = ch | ((unsigned char) pow2);
				pow2 >>= 1;
				bit_counter++;
				
				if(bit_counter == 8) {
					bit_counter = 0;
					fputc(ch, fd);
					ch = 0;
					pow2 = 0x80;
				}
			}
			if(bit_counter!=0)
				printf("WARNING: lsbextract.c in lsbextract(): bit_counter != 0\n");
		}
	}
}