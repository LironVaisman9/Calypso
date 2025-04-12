#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H

#include "stb_image.h"
#include "stb_image_write.h"

#include <stdbool.h>

bool writeToImage(char* format,char* path, unsigned char* data, int width, int height, int channels);

#endif
