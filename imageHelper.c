#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "imageHelper.h"
#include <string.h>
#include "macros.h"

bool writeToImage(char* format,char* path, unsigned char* data, int width, int height, int channels)
{
    if (strcmp(format, "png") == 0 || strcmp(format, "PNG") == 0)
    {
        return stbi_write_png(path, width, height, channels, data, width * channels);
    }
    else if(strcmp(format, "jpg") == 0 || strcmp(format, "JPG") == 0 || strcmp(format, "jpeg") == 0 || strcmp(format, "JPEG") == 0)
    {
        return stbi_write_jpg(path, width, height, channels, data, JPG_QUALITY);
    }
    else if(strcmp(format, "bmp") == 0 || strcmp(format, "BMP") == 0)
    {
        return stbi_write_bmp(path, width, height, channels, data);
    }
    else if(strcmp(format, "tga") == 0 || strcmp(format, "TGA") == 0)
    {
        return stbi_write_tga(path, width, height, channels, data);
    }
    LOG_ERROR("'%s' is an unsupported file format",format);
    return false;
}