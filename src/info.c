#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "psteg.h"
#include "math.h"
#include "createPNG.h"

int countColors(unsigned char*, int, int, int);
void lsbanalysis(unsigned char*, int, int, int);

void info(char* filename) {
	int width, height, depth;
	unsigned char *data = stbi_load(filename, &width, &height, &depth, 4); //force alpha channel

	//print general informations
	printf("width: %d\n", width);
	printf("height: %d\n", height);
	printf("depth: %d\n", depth);
	if(depth == 4)
		printf("the image has an alpha channel\n");

	//counting colors
	countColors(data, width, height, depth);

	//lsb analysis
	lsbanalysis(data, width, height, depth);
}

//counts colors without alpha channel
int countColors(unsigned char* data, int width, int height, int depth) {
	long nbytes = pow(2, 24)/8;
	unsigned char *colormap = (unsigned char*) malloc(nbytes);
	memset(colormap, 0, nbytes);

	long numpixels = width*height;
	int index;
	char mask;
	if(sizeof(int) != 4) {
		printf("ERROR: info.c in countColors(). sizeof(int) != 4\n");
		return 0;
	}
	int i;
	int pixel;
	for(i=0; i!=numpixels; i++) {
		pixel = *((int*)(data[i*4]));
		index = (pixel >> 4)/8;
		mask = (pixel >> 4)%8;
		colormap[index] |= mask;
	}

	long acc=0;
	char j;
	for(i=0; i!=nbytes; i++) {
		j=0x01;
		do {
			acc+=(colormap[i] & j)?1:0;
			j <<= 1;
		} while(j!=0x80);
	}

	printf("Number of unique colors: %ld\n", acc);

	free(colormap);

	return 1;
}

void lsbanalysis(unsigned char* data, int width, int height, int depth) {
	unsigned char *buffer, *buffer_ptr;
	int i, j;
	
	buffer = (unsigned char*) malloc(width * height * 3);
	
	long npixels = width*height;
	for(i=0; i!=npixels; i++)
		for(j=0; j!=3; j++)
			buffer[(i*3)+j] = (data[(i*4)+j] & 0x01) * 255;
	
	if(writeImage("LSB_analisys.png", width, height, buffer, "LSB_analisys"))
		printf("LSB_analisys.png created\n");
	else
		printf("something went wrong creating the png\n");

	free(buffer);
}