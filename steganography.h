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
    uint16_t m_textSize;
    size_t m_startPos;
}Header;

typedef struct Tail
{
    char m_magic[MAGIC_LENGTH + 1];    
    char m_format[MAX_FORMAT_LENGTH];
    size_t m_offset;      
    size_t m_size;       
}Tail;

size_t findstartingPos(unsigned char* data,size_t dataSize,uint16_t length);

char* createMagic(FileObject* file);
bool headerValid(Header* header,char* magic);
Header* createHeader(FileObject* file,uint16_t textSize,size_t startingPos);

bool insertHeader(unsigned char* dest,Header* header);
Header* getHeader(unsigned char* src);

bool tailValid(Tail* tail,char* magic);
Tail* getTail(unsigned char* src,size_t size);
Tail* createTail(FileObject* file,char* format,size_t offset,size_t size);

bool encodeLSB(unsigned char* dest,unsigned char* data,int startPos,int size);
bool decodeLSB(unsigned char* src,unsigned char* dest,int startPos,int size);

bool encodeMessage(FileObject* destFile,char* msg);
bool decodeMessage(FileObject* srcFile,char** msg);

bool encodeFile(FileObject* destFile,FileObject* file);
bool decodeFile(FileObject* srcFile,char* path);

bool hasHeader(FileObject* file);
bool hasTail(FileObject* file);

void encode(FileObject* file);
void decode(FileObject* file);

#endif