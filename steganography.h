#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include <stdint.h>
#include "fileManager.h"

#define MAGIC_LENGTH 4
#define HEADER_STARTING_BYTE 150
#define ENCODE_TEXT 1
#define ENCODE_FILE 2
#define MAX_TEXT_LENGTH 250
#define MAX_PATH_LENGTH 256

typedef struct Header
{
    char m_magic[MAGIC_LENGTH + 1];
    char m_type;
    char m_fileFormat[MAX_FORMAT_LENGTH];
    uint16_t m_textSize;
    size_t m_startPos;
}Header;

size_t findstartingPos(unsigned char* data,size_t dataSize,uint16_t length);

char* createMagic(FileObject* file);
bool headerValid(Header* header,char* magic);
Header* createHeader(FileObject* file,char type,char* fileFormat,uint16_t textSize,size_t startingPos);

bool insertHeader(unsigned char* dest,Header* header);
Header* getHeader(unsigned char* src);

bool encodeLSB(unsigned char* dest,unsigned char* data,int startPos,int size);
bool decodeLSB(unsigned char* src,unsigned char* dest,int startPos,int size);

bool encodeMessage(FileObject* destFile,char* msg);
bool decodeMessage(FileObject* srcFile,char** msg);

bool encodeFile(FileObject* destFile,FileObject* file);
bool decodeFile(FileObject* srcFile,char* path);

void encode(FileObject* file);
void decode(FileObject* file);

#endif