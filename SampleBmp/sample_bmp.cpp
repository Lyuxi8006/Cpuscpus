#include "bmp.h"
#include <stdlib.h>
int main()
{

	const char* r_fileName = "E:\\Lyrics\\Project\\SempleBmp\\lena_gray.bmp";
	const char* w_fileName = "E:\\Lyrics\\Project\\SempleBmp\\lena_gray_r.bmp";
	const char* fileName = "E:\\Vinnord\\log\\mydata";

	size_t width = 0;
	size_t height = 0;
	uint8_t* pixels = nullptr;
	//int res = read_bmpfile(r_fileName, &width, &height, &pixels);
	int res = read_data(fileName, &width, &height, &pixels);
	if (res == 0)
	{
		write_bmpfile(w_fileName, width, height, pixels);
		free(pixels);
		pixels = nullptr;
	}
	return 0;
}
