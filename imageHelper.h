#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H

#include <stdbool.h>

#define JPG_QUALITY 100

unsigned char* loadImage(const char* path, int* width, int* height, int* channels);
bool writeToImage(char* format,char* path, unsigned char* data, int width, int height, int channels);
void freeImageData(unsigned char* data);

#endif
