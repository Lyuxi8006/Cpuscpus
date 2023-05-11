#include "bmp.h"
#include <stdio.h>
#include <string.h>


#ifdef _MSC_VER
__pragma(pack(push, 1))
struct bitmap_file_header_t
{
	/** Magic identifier: "BM" (for bmp files)*/
	uint16_t magic;
	/** Size of the bmp file in bytes */
	uint32_t fileSize;
	/** Reserved 1 */
	uint16_t reserved1;
	/** Reserved 2 */
	uint16_t reserved2;
	/** Data offset relative to the start of the bmp data */
	uint32_t dataOffset;
};
struct bitmap_info_header_t
{
	/** Header size in bytes */
	uint32_t headerSize;
	/** Width of bmp */
	uint32_t width;
	/** Height of bmp */
	uint32_t height;
	/** Color planes */
	uint16_t colorPlanes;
	/** Color depth */
	uint16_t bitsPerPixel;
	/** Compression type */
	uint32_t compressionType;
	/** Size of raw BMP data */
	uint32_t imageDataSize;
	/** Horizontal resolution (pixels per meter) */
	uint32_t hPixelsPerMeter;
	/** Vertical resolution (pixels per meter) */
	uint32_t vPixelsPerMeter;
	/** Number of color indices in the color table that are actually used by the bitmap */
	uint32_t colorsUsed;
	/** Number of color indices that are required for displaying the bitmap */
	uint32_t colorsRequired;
};


struct palette_t
{
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
};

#pragma pack(pop)

#elif defined(__GNUC__)
struct bitmap_file_header_t
{
	/** Magic identifier: "BM" (for bmp files)*/
	uint16_t magic;
	/** Size of the bmp file in bytes */
	uint32_t fileSize;
	/** Reserved 1 */
	uint16_t reserved1;
	/** Reserved 2 */
	uint16_t reserved2;
	/** Data offset relative to the start of the bmp data */
	uint32_t dataOffset;
}__attribute__((packed));
struct bitmap_info_header_t
{
	/** Header size in bytes */
	uint32_t headerSize;
	/** Width of bmp */
	uint32_t width;
	/** Height of bmp */
	uint32_t height;
	/** Color planes */
	uint16_t colorPlanes;
	/** Color depth */
	uint16_t bitsPerPixel;
	/** Compression type */
	uint32_t compressionType;
	/** Size of raw BMP data */
	uint32_t imageDataSize;
	/** Horizontal resolution (pixels per meter) */
	uint32_t hPixelsPerMeter;
	/** Vertical resolution (pixels per meter) */
	uint32_t vPixelsPerMeter;
	/** Number of color indices in the color table that are actually used by the bitmap */
	uint32_t colorsUsed;
	/** Number of color indices that are required for displaying the bitmap */
	uint32_t colorsRequired;
}__attribute__((packed));


struct palette_t
{
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
}__attribute__((packed));

#endif



void draw_line(int sx, int sy, int dx, int dy, uint8_t color, uint8_t* pixels, int lpitch)
{

	int deta_x = dx - sx;
	int deta_y = dy - sy;

	int sign_x = deta_x > 0 ? 1 : -1;
	int sign_y = deta_y > 0 ? 1 : -1;


	int x = sx;
	int y = sy;

	for (int ix = 0, iy = 0; ix < deta_x || iy < deta_y;)
	{

		//这里(0.5)表示像素中间
		//if ((0.5 + ix) /deta_x < (0.5 + iy) / deta_y) 不等式两边同时乘以2 * deta_x * deta_y 去掉浮点数
		int decision = (1 + 2 * ix) * deta_y - (1 + 2 * iy) * deta_x;
		if (decision == 0)
		{
			x += sign_x;
			y += sign_y;
			ix++;
			iy++;
		}
		else if (decision < 0)
		{
			x += sign_x;
			ix++;
		}
		else
		{
			y += sign_y;
			iy++;
		}
		pixels[x + y * lpitch] = color;
	}
}

int read_data(const char* fileName, size_t* width, size_t* height, float* cmPerPixel, uint8_t** pixels,
	int* sx, int* sy, int* dx, int* dy)
{
	FILE* fp = fopen(fileName, "rb");
	if (fp == nullptr)
	{
		return -1;
	}
	fseek(fp, 0, SEEK_END);

	size_t file_size = ftell(fp);
	rewind(fp);
	uint8_t* buffer = (uint8_t*)malloc(file_size);

	if (buffer == nullptr)
	{
		fclose(fp);
		fp = nullptr;
		return -1;
	}

	fread(buffer, file_size, 1, fp);


	size_t _height = *(int32_t*)buffer;
	size_t _width = *(int32_t*)(buffer + 4);
	int _sx = *(int*)(buffer + 8);
	int _sy = *(int*)(buffer + 12);
	int _dx = *(int*)(buffer + 16);
	int _dy = *(int*)(buffer + 20);
	float _cmPerPixel = *(float*)(buffer + 24);

	size_t total_size = _height * _width;

	if (total_size + 28 != file_size)
	{

		free(buffer);
		buffer = nullptr;
		fclose(fp);
		fp = nullptr;
		return -1;
	}

	uint8_t* _pixels = (uint8_t*)malloc(_width * _height);

	if (_pixels == nullptr)
	{
		free(buffer);
		buffer = nullptr;
		fclose(fp);
		fp = nullptr;
		return -1;
	}

	//for (int i = 0; i < _height; ++i)
	//{
	//	for (int j = 0; j < _width; ++j)
	//	{
	//		_pixels[i * _width + j] = buffer[(_height - i - 1) * _width + j + 8];
	//	}
	//}

	memcpy(_pixels, &buffer[28], _width * _height);



	*width = _width;
	*height = _height;
	*cmPerPixel = _cmPerPixel;
	*sx = _sx;
	*sy = _sy;
	*dx = _dx;
	*dy = _dy;
	*pixels = _pixels;
	free(buffer);
	buffer = nullptr;
	fclose(fp);
	fp = nullptr;
	return 0;

}

int read_bmpfile(const char* fileName, size_t* width, size_t* height, uint8_t** pixels)
{

	FILE* fp = fopen(fileName, "rb");
	if (fp == nullptr)
	{
		return -1;
	}
	fseek(fp, 0, SEEK_END);

	size_t file_size = ftell(fp);
	rewind(fp);

	uint8_t* buffer = (uint8_t*)malloc(file_size);

	if (buffer == nullptr)
	{
		fclose(fp);
		fp = nullptr;
		return -1;
	}

	fread(buffer, file_size, 1, fp);

	size_t file_header_size = sizeof(bitmap_file_header_t);
	size_t info_header_size = sizeof(bitmap_info_header_t);

	bitmap_file_header_t* file_header = (bitmap_file_header_t*)buffer;
	bitmap_info_header_t* info_header = (bitmap_info_header_t*)(buffer + file_header_size);

	if (info_header->bitsPerPixel != 8)
	{
		free(buffer);
		buffer = nullptr;
		fclose(fp);
		fp = nullptr;
		return -1;
	}
	size_t palette_count = 256;

	palette_t* palette = (palette_t*)(buffer + file_header_size + info_header_size);

	//
	palette_t* p = palette;
	for (int i = 0; i < palette_count; ++i, ++p)
	{
		uint32_t b = p->rgbBlue;
		uint32_t g = p->rgbGreen;
		uint32_t r = p->rgbRed;
		uint32_t a = p->rgbReserved;
		if (i % 4 == 0 && i != 0)
		{
			// printf("bgra (%u %u %u %u)\n", b, g, r, a);
		}
		else
		{
			// printf("bgra (%u %u %u %u) ", b, g, r, a);
		}
	}

	size_t palette_size = palette_count * sizeof(palette_t);

	size_t data_offset = file_header_size + info_header_size + palette_size;




	size_t width_pad_size = (info_header->width & 3);
	size_t pad_width = info_header->width + width_pad_size;

	size_t image_data_size = pad_width * info_header->height;

	uint8_t* _pixels = (uint8_t*)malloc(info_header->height * info_header->width);

	if (_pixels == nullptr)
	{
		free(buffer);
		buffer = nullptr;
		fclose(fp);
		fp = nullptr;
		return -1;
	}

	for (size_t i = 0; i < info_header->height; ++i)
	{
		for (size_t j = 0; j < info_header->width; ++j)
		{
			_pixels[i * info_header->width + j] = buffer[data_offset + i * pad_width + j];
		}
	}


	size_t total_size = file_header_size + info_header_size + palette_size + image_data_size;


	size_t file_pad_size = (total_size & 3);
	total_size += file_pad_size;

	if (total_size != file_size)
	{
		free(_pixels);
		_pixels = nullptr;
		free(buffer);
		buffer = nullptr;
		fclose(fp);
		fp = nullptr;
		return -1;
	}
	*width = info_header->width;
	*height = info_header->height;
	*pixels = _pixels;
	free(buffer);
	buffer = nullptr;
	fclose(fp);
	fp = nullptr;
	return 0;
}


int write_bmpfile(const char* fileName, size_t width, size_t height, uint8_t* pixels)
{

	FILE* fp = fopen(fileName, "wb");
	if (fp == nullptr)
	{
		return -1;
	}

	size_t file_header_size = sizeof(bitmap_file_header_t);
	size_t info_header_size = sizeof(bitmap_info_header_t);

	size_t palette_count = 256;

	size_t palette_size = palette_count * sizeof(palette_t);


	size_t data_offset = file_header_size + info_header_size + palette_size;

	size_t width_pad_size = (width & 3);

	size_t pad_width = width + width_pad_size;

	size_t image_data_size = pad_width * height;



	size_t total_size = file_header_size + info_header_size + palette_size + image_data_size;

	size_t file_pad_size = (total_size & 3);
	total_size += file_pad_size;

	size_t file_size = total_size;

	uint8_t* buffer = (uint8_t*)malloc(file_size);

	if (buffer == NULL)
	{
		fclose(fp);
		fp = nullptr;
		return -1;
	}

	bitmap_file_header_t* file_header = (bitmap_file_header_t*)buffer;
	bitmap_info_header_t* info_header = (bitmap_info_header_t*)(buffer + file_header_size);

	memset(file_header, 0, sizeof(bitmap_file_header_t));
	file_header->magic = 0x4D42;
	file_header->dataOffset = data_offset;
	file_header->fileSize = file_size;

	memset(info_header, 0, sizeof(bitmap_info_header_t));
	info_header->headerSize = sizeof(bitmap_info_header_t);
	info_header->width = width;
	info_header->height = (~height + 1);
	info_header->colorPlanes = 1;
	info_header->bitsPerPixel = 8;
	info_header->imageDataSize = 0;


	palette_t* palette = (palette_t*)(buffer + file_header_size + info_header_size);

	palette_t* p = palette;
	for (size_t i = 0; i < palette_count; ++i, ++p)
	{
		p->rgbBlue = i;
		p->rgbGreen = i;
		p->rgbRed = i;
		p->rgbReserved = 0;
	}

	uint8_t* data = buffer + data_offset;

	for (size_t i = 0; i < height; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			data[i * pad_width + j] = pixels[i * width + j];
		}
		//���벹0
		for (size_t k = 0; k < width_pad_size; ++k)
		{
			data[i * pad_width + width + k] = 0;
		}
	}
	//�ļ�����
	for (size_t i = 0; i < file_pad_size; ++i)
	{
		buffer[total_size - i - 1] = 0;
	}
	fwrite(buffer, total_size, 1, fp);
	free(buffer);
	buffer = nullptr;
	fclose(fp);
	fp = nullptr;
	return 0;
}
