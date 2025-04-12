#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include <stdint.h>
#include "fileManager.h"

#define MAGIC_LENGTH 4
#define HEADER_STARTING_BYTE 100

typedef struct Header
{
    char m_magic[MAGIC_LENGTH + 1];
    char m_type;
    char m_fileFormat[MAX_FORMAT_LENGTH];
    uint16_t m_textSize;
    size_t m_startPos;
}Header;

char* createMagic(FileObject* file);
Header* createHeader(FileObject* file);

bool insertHeader(unsigned char* dest,Header* header);
Header* getHeader(unsigned char* src);

bool encode(unsigned char* dest,unsigned char* data,int startPos,int size);
bool decode(unsigned char* src,unsigned char* dest,int startPos,int size);

void encodeMessage(FileObject* destFile,char* msg);
void decodeMessage(FileObject* srcFile,char* msg);

void encodeFile(FileObject* destFile,FileObject* file);
void decodeFile(FileObject* srcFile,char* path);

#endif