#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#ifdef _MSC_VER
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif



int main()
{
	char buff[FILENAME_MAX];
	char* cwd = getcwd(buff, FILENAME_MAX);
	if (cwd == nullptr)
	{
		return -1;
	}
	std::string current_dir_name = cwd;
	current_dir_name += "/";

	std::cout << current_dir_name << std::endl;
	const std::string r_file_name = current_dir_name + "lena_gray.bmp";
	const std::string w_file_name = current_dir_name + "lena_gray_r.bmp";
	const std::string file_name = current_dir_name + "mydata";

	size_t width = 0;
	size_t height = 0;
	uint8_t* pixels = nullptr;
	int res = read_bmpfile(r_file_name.c_str(), &width, &height, &pixels);
	//int res = read_data(file_name.c_str(), &width, &height, &pixels);
	if (res == 0)
	{
		write_bmpfile(w_file_name.c_str(), width, height, pixels);
		free(pixels);
		pixels = nullptr;
	}
	return 0;
}
