#ifndef _BMP_H_
#define _BMP_H_
#include <stdlib.h>
#include <stdint.h>

int read_data(const char* fileName, size_t* width, size_t* height, float* cmPerPixel, uint8_t** pixels,
	int* sx, int* sy, int* dx, int* dy);
int read_bmpfile(const char* fileName, size_t* width, size_t* height, uint8_t** pixels);
int write_bmpfile(const char* fileName, size_t width, size_t height, uint8_t* pixels);
void draw_line(int sx, int sy, int dx, int dy, uint8_t color, uint8_t* pixels, int lpitch);


#endif /* _BMP_H_ */

