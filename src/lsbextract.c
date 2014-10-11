#include <stdio.h>
#include <stdlib.h>
#include "psteg.h"
#include "string.h"
#include "math.h"
#include "stb_image.h"

void lsbextract(char* filename, char* orderstring, char nbit) {
	int width, height, depth;
	printf("%s, %s, %d\n", filename, orderstring, nbit);
	unsigned char *data = stbi_load(filename, &width, &height, &depth, 0);
	if(data == NULL) {
		printf("ERROR: lsbextract.c in lsbextract(): stbi_load() failed\n");
		exit(-1);
	}

	int i, j, chan_num;
	unsigned char byte;
	char ch = 0;
	FILE *fd;
	
	chan_num = strnlen(orderstring, 4);

	fd = fopen(orderstring, "w");

	unsigned char k;
	unsigned char bitmask = 0x80;
	long pixels = width*height;
	for(i=0; i!=pixels; i++) {
		for(j=0; j!=chan_num; j++) {
			if(orderstring[j] == 'R' || orderstring[j] == 'r')
				byte = data[i*depth];
			else if(orderstring[j] == 'G' || orderstring[j] == 'g')
				byte = data[(i*depth)+1];
			else if(orderstring[j] == 'B' || orderstring[j] == 'b')
				byte = data[(i*depth)+2];
			else if(orderstring[j] == 'A' || orderstring[j] == 'a')
				if(depth < 4)
					printf("WARNING: lsbextract.c in lsbextract(): ignoring a channel extraction: no alpha channel\n");
				else
					byte = data[(i*depth)+3];

			for(k=0x01 << nbit-1; k!=0; k>>=1) {
				if(byte & k)
					ch = ch | ((unsigned char) bitmask);
				bitmask >>= 1;
				
				if(bitmask == 0) {
					fputc(ch, fd);
					ch = 0;
					bitmask = 0x80;
				}
			}
		}
	}
	if(bitmask!=0)
		printf("WARNING: lsbextract.c in lsbextract(): bitmask != 0\n");

	close(fd);
	printf("file ceated\n");

	stbi_image_free(data);
}