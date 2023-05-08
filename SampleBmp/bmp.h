fndef __BMP_H_
#define __BMP_H_
#include <stdint.h>

int read_data(const char* fileName, size_t* width, size_t* height, uint8_t** pixels);
int read_bmpfile(const char* fileName, size_t* width, size_t* height, uint8_t** pixels);
int write_bmpfile(const char* fileName, size_t width, size_t height, uint8_t* pixels);


#endif /* __BMP_H_ */

