#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <string.h>
#ifdef _MSC_VER
#include <direct.h>
#include <io.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#include <dirent.h>
#endif

#ifdef _MSC_VER
// ��������������ټ�һ��format��ʽ������push��vectorǰ�ж����ļ�����׺��������ָ����ʽ
void GetAllFiles(std::string path, std::vector<std::string>& files) {
	// �����洢�ļ���Ϣ�Ľṹ�壬��ͷ�ļ� <io.h>
	struct _finddata_t fileinfo;  // _finddata_t ����һ��struct�࣬c++�п��Բ�Ҫǰ���struct��

	intptr_t hFile = 0;

	std::string p;  // ������p(path)��ʼ��
	// ��һ�β���
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			// ����ҵ������ļ���
			if ((fileinfo.attrib & _A_SUBDIR)) {
				// ��������ļ��У���������continue
				if (std::strcmp(fileinfo.name, ".") != 0 && std::strcmp(fileinfo.name, "..") != 0) {
					// �������
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			// ����ҵ��Ĳ����ļ���
			else {
				// ��������ļ���
				files.push_back(p.assign(fileinfo.name));
				// Ҳ�����Ǳ������·��
				// files.push_back(p.assign(path).append("\\").append(fileinfo.name));  
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		// ��������
		_findclose(hFile);
	}

}
#else
void GetAllFiles(std::string path, std::vector<std::string>& files) {
	DIR* pDir;   //  ��ͷ�ļ�<dirent.h>������
	struct dirent* ptr;  // opendir��readdir��Щ����ͷ�ļ�dirent.h
	if (!(pDir = opendir(path.c_str()))) return;
	while ((ptr = readdir(pDir)) != 0) {
		// strcmp��C������ģ�ֻ����string,Ȼ��std::strcmp����û�е�
		if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
			files.push_back(path + "/" + ptr->d_name);  // ����ֻ��������
		}
	}
	closedir(pDir);
}
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
	int sx = 0;
	int sy = 0;
	int dx = 0;
	int dy = 0;
	float cmPerPixel = 0.00f;
	uint8_t* pixels = nullptr;
	//int res = read_bmpfile(r_file_name.c_str(), &width, &height, &pixels);

	std::vector<std::string> files;
	GetAllFiles(current_dir_name, files);
	for (size_t i = 0; i < files.size(); ++i)
	{
		std::string& data_file_name = files[i];
		std::string sub_str("mydata");
		std::size_t found = data_file_name.find(sub_str);
		if (found == std::string::npos)
		{
			continue;
		}
		int res = read_data(data_file_name.c_str(), &width, &height, &cmPerPixel, &pixels, &sx, &sy, &dx, &dy);
		if (res == 0)
		{
			draw_line(sx, sy, dx, dy, 255, pixels, width);
			std::string bmp_file = data_file_name + ".bmp";
			write_bmpfile(bmp_file.c_str(), width, height, pixels);
			free(pixels);
			pixels = nullptr;
		}
	}

	return 0;
}
