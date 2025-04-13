#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "imageHelper.h"
#include <string.h>
#include "macros.h"

/// @brief loads the binary data of an image
/// @param path the path to the image
/// @param width the width of the image
/// @param height the height of the image
/// @param channels the color channels of the image(RGB for example)
/// @return the binary data of the image
unsigned char* loadImage(const char* path, int* width, int* height, int* channels)
{
    return stbi_load(path, width, height, channels, 0);
}
/// @brief writes an image to memory
/// @param format the format of the image
/// @param path the path of the image
/// @param data the binary data of the image
/// @param width the width of the image
/// @param height the height of the image
/// @param channels the color channels of the image(RGB for example)
/// @return true if the process was successful and false if no
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
/// @brief frees memory allocated for binary data of an image
/// @param data the binary data of the image
void freeImageData(unsigned char* data)
{
    stbi_image_free(data);
}