#include "steganography.h"
#include "macros.h"
#include "stringFunctions.h"
#include <stdlib.h>
#include <string.h>

char* createMagic(FileObject* file)
{
    char* magic = NULL;
    magic = (char*)malloc(sizeof(char) * (MAGIC_LENGTH + 1));
    if (magic == NULL)
    {
        LOG_ERROR("Could not allocate memory for magic");
        return NULL;
    }
    unsigned int seed = (unsigned int)strlen(file->m_name);
    randomString(magic,MAGIC_LENGTH + 1,seed,true);
    return magic;
}

bool insertHeader(unsigned char* dest,Header* header)
{
    unsigned char* binaryHeader = (unsigned char*)malloc(sizeof(Header));
    if (binaryHeader == NULL)
    {
        LOG_ERROR("Could not allocate memory for binary header");
        return false;
    }
    memcpy(binaryHeader, header, sizeof(Header));
    if(!encode(dest,binaryHeader,HEADER_STARTING_BYTE,sizeof(Header)))
    {
        LOG_ERROR("Could not encode header into the image");
        free(binaryHeader);
        return false;
    }
    free(binaryHeader);
    return true;
}
Header* getHeader(unsigned char* src)
{
    Header* header = NULL;
    unsigned char* binaryHeader = NULL;
    binaryHeader = (unsigned char*)malloc(sizeof(Header));
    if (binaryHeader == NULL)
    {
        LOG_ERROR("Could not allocate memory for binary header");
        return NULL;
    }
    if (!decode(src,binaryHeader,HEADER_STARTING_BYTE,sizeof(Header)))
    {
        LOG_ERROR("Could not read steganography from file");
        free(binaryHeader);
        return false;
    }
    header = (Header*)malloc(sizeof(Header));
    if (header == NULL)
    {
        LOG_ERROR("Could not allocate memory for the steganography header");
        free(binaryHeader);
        return NULL;
    }
    memcpy(header, binaryHeader, sizeof(Header));
    free(binaryHeader);
    return header;
}
bool encode(unsigned char* dest,unsigned char* data,int startPos,int size)
{
    int bitsWritten = 0;
    int i = 0;
    for (i = 0; i < size; i++)
    {
        unsigned char byte = data[i];
        int bit = 0;
        for (bit = 7; bit >= 0; bit--)
        {
            int bitToWrite = (byte >> bit) & 1;
            dest[startPos + bitsWritten] &= 0xFE;
            dest[startPos + bitsWritten] |= bitToWrite;
            bitsWritten++;
        }
    }
    if (bitsWritten != (size * 8))
    {
        LOG_ERROR("Could not write the entire data to the dest");
        return false;
    }
    return true;
}
bool decode(unsigned char* src,unsigned char* dest,int startPos,int size)
{
    int bitsRead = 0;
    int i = 0;
    for (i = 0; i < size; i++)
    {
        unsigned char byte = 0;
        int bit = 0;
        for (bit = 7; bit >= 0; bit--)
        {
            int bitFromSrc = src[startPos + bitsRead] & 1;
            byte |= (bitFromSrc << bit);
            bitsRead++;
        }
        dest[i] = byte;
    }
    if (bitsRead != (size * 8))
    {
        LOG_ERROR("Could not read all the binary data from src");
        return false;
    }
    return true;
}