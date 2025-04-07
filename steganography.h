#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

void encodeMessage(unsigned char* dest,char* msg);
void decodeMessage(unsigned char* src,char* dest);
void encode(unsigned char* dest,unsigned char* data);
void decode(unsigned char* src,unsigned char* dest);

#endif