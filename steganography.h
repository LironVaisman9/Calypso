#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include "fileManager.h"

void encode(unsigned char* dest,unsigned char* data);
void decode(unsigned char* src,unsigned char* dest);

void encodeMessage(FileObject* destFile,char* msg);
void decodeMessage(FileObject* srcFile,char* dest);

void encodeFile(FileObject* destFile,FileObject* file);
void decodeFile(FileObject* srcFile,char* path);

#endif