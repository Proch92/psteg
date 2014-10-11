#include "psteg.h"
#include <stdio.h>
#include <stdlib.h>
#include "createPNG.h"
#include "stb_image.h"

void show_alpha_layer(unsigned char*, int, int, unsigned char*);

void alpha(char* filename) {
	int width, height, depth;
	unsigned char *data = stbi_load(filename, &width, &height, &depth, 0);
	if(data == NULL) {
		printf("ERROR: alpha.c in alpha(): stbi_load() failed\n");
		exit(-1);
	}

	if(depth < 4) {
		printf("No alpha channel\n");
		return;
	}

	int i;
	unsigned char alpha_values[256];
	unsigned char values_found = 0;
	
	for(i=0; i!=256; i++)
		alpha_values[i] = 0;
	
	long pixels = width * height;
	for(i=0; i!=pixels; i++)
		alpha_values[data[(i*4)+3]] = 1;
	
	for(i=0; i!=256; i++)
		if(alpha_values[i])
			values_found++;
	
	if(values_found != 1) {
		printf("irregularity were found on alpha channel\n%d different alpha values found\n", values_found);
		show_alpha_layer(data, width, height, alpha_values);
	}
	else
		printf("irregularity were not found on alpha channel\n");

	stbi_image_free(data);
}

/*
make a b/w png image showing alpha layer with highest contrast
*/
void show_alpha_layer(unsigned char* data, int width, int height, unsigned char* values) {
	int i;
	unsigned char *pix;
	unsigned char min, max, range;
	unsigned char *buffer;
	
	for(i=0; i!=256; i++)
		if(values[i]) {
			min = i;
			break;
		}
	
	for(i=255; i>=0; i--)
		if(values[i]) {
			max = i;
			break;
		}
	
	range = max - min;
	
	buffer = (unsigned char*) malloc(width * height * 3);
	
	int color;
	long pixels = width * height;
	for(i=0; i!=pixels; i++) {
		color = (((data[(i*4)+3] - min) * 255) / range);
		buffer[(i*3)+0] = color;
		buffer[(i*3)+1] = color;
		buffer[(i*3)+2] = color;
	}
	
	if(writeImage("alpha_layer.png", width, height, buffer, "alpha_analisys"))
		printf("alpha_layer.png created\n");
	else
		printf("something went wrong creating the png\n");

	free(buffer);
}