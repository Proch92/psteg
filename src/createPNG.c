#include "stdlib.h"
#include "png.h"
#include "createPNG.h"

int writeImage(char* filename, int width, int height, png_byte *buffer, char* title) {
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep row;
	
	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) return 0;

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) return 0;

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) return 0;

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) return 0;

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep) malloc(3 * width * sizeof(png_byte));

	// Write image data
	int i, j, k;
	png_bytep buffer_ptr = buffer;
	png_bytep row_ptr = row;
	for (i=0 ; i<height ; i++) {
		for (j=0 ; j<width ; j++) {
			for(k=0; k!=3; k++) {
				*row_ptr = *buffer_ptr;
				row_ptr++;
				buffer_ptr++;
			}
		}
		png_write_row(png_ptr, row);
		row_ptr = row;
	}

	// End write
	png_write_end(png_ptr, NULL);

	return 1;
}
