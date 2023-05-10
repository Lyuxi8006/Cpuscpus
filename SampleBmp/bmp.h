#ifndef _BMP_H_
#define _BMP_H_
#include <stdlib.h>
#include <stdint.h>

int read_data(const char* fileName, size_t* width, size_t* height, float *cmPerPixel, uint8_t** pixels);
int read_bmpfile(const char* fileName, size_t* width, size_t* height, uint8_t** pixels);
int write_bmpfile(const char* fileName, size_t width, size_t height, uint8_t* pixels);


#endif /* _BMP_H_ */

